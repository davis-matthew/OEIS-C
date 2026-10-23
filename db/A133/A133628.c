/*
 * A133628  a(1)=1, a(n) = a(n-1) + (p-1)*p^(n/2-1) if n is even, else a(n) =
 *          a(n-1) + p^((n-1)/2), where p=4.
 * https://oeis.org/A133628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1 a(n) = a(n-1) + 3*4^(n/2-1) if n is even a(n) = a(n-1)
 *            + 4^((n-1)/2) if n is odd (Equivalently: a(n) = a(n-1) + (n
 *            even?3:1) * 4^⌊(n‑1)/2⌋) where p = 4 and ⌊x⌋ is the integer
 *            floor
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* undefined for n < 1, harmless */

    seq_t result = (seq_t)1;   /* a(1) */
    seq_t pow4   = (seq_t)1;   /* 4^0  = 1, exponent floor((i-1)/2) for i=2 */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term = pow4;
        if ((i & (idx_t)1) == 0)   /* even i => multiply by 3 */
            term *= (seq_t)3;

        result += term;

        if ((i & (idx_t)1) == 0)   /* after an even step, advance exponent */
            pow4 *= (seq_t)4;
    }

    return result;
}
