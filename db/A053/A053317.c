/*
 * A053317  a(n) contains n digits (either '2' or '5') and is divisible by
 *          2^n.
 * https://oeis.org/A053317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + 10^(n-1)*(2 + 3*(a(n-1)/2^(n-1) mod 2)) Initial
 *            value: a(1) = 2 (Recurrence: a(n) = a(n-1) + 10^(n-1)*d, d = 2
 *            if a(n-1) divisible by 2^n, else d = 5)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* not used – offset is 1 */

    seq_t a_n = (seq_t)2;          /* a(1) */
    if (n == 1)
        return a_n;

    seq_t pow10 = (seq_t)1;        /* 10^(i-1) for current i */
    seq_t pow2  = (seq_t)2;        /* 2^i for current i */

    for (idx_t i = 2; i <= n; ++i)
    {
        pow10 *= (seq_t)10;       /* 10^(i-1) */
        pow2  <<= 1;              /* 2^i      */

        /* digit d: 2 if a_n % 2^i == 0 else 5 */
        seq_t digit = ((a_n & (pow2 - (seq_t)1)) == 0) ? (seq_t)2 : (seq_t)5;

        a_n += digit * pow10;
    }

    return a_n;
}
