/*
 * A064352  a(n) = (3*n)!/(2*n)!.
 * https://oeis.org/A064352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n)!/(2*n)! = ∏_{k=1..n} (2*n + k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;          /* result */
    seq_t m = (seq_t)n;     /* convert n to seq_t */
    seq_t base = m * 2;     /* 2*n as seq_t */

    for (idx_t i = 1; i <= n; ++i)
    {
        res *= (base + (seq_t)i);
    }
    return res;
}
