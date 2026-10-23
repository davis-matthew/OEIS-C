/*
 * A033439  Number of edges in 7-partite Turán graph of order n.
 * https://oeis.org/A033439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3*n^2/7)
 * Range:     u8 n <= 24, u16 n <= 391, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
#if defined(__SIZEOF_INT128__)
    /* Use a 128-bit intermediate to avoid overflow when n is large */
    unsigned __int128 temp = (unsigned __int128)n * n * 3;
    return (seq_t)(temp / 7);
#else
    /* Fallback: rely on the native type; works fine for n ≤ 10⁴ */
    seq_t nn = (seq_t)n;
    return nn * nn * 3 / 7;
#endif
}
