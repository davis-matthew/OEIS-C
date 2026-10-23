/*
 * A164602  a(n) = ((1+4*sqrt(2))*(1+2*sqrt(2))^n +
 *          (1-4*sqrt(2))*(1-2*sqrt(2))^n)/2.
 * https://oeis.org/A164602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) a(0) = 1, a(1) = 17
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)17;

    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)17; /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 * (seq_t)2 + prev2 * (seq_t)7;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
