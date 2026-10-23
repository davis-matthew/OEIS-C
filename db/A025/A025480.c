/*
 * A025480  a(2n) = n, a(2n+1) = a(n).
 * https://oeis.org/A025480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice, tabf, hear
 * Formula:   a(2n) = n and a(2n+1) = a(n) (OEIS A025480)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* keep removing the lowest 1‑bit until n is even,
       then divide by 2.  Works for all n ≥ 0. */
    while (n & 1ULL)
        n >>= 1;
    return (seq_t)(n >> 1);
}
