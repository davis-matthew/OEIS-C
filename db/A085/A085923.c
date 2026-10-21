/*
 * A085923  a(0) = 1, a(n+1) = (n+1)*(a(n) + n).
 * https://oeis.org/A085923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n+1) = (n+1)*(a(n)+n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;          // a(0)
    if (n == 0) return val;

    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        /* Compute a(i) = i * (a(i-1) + i-1) */
        val = (seq_t)i * (val + (seq_t)(i - (idx_t)1));
    }
    return val;
}
