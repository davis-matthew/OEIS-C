/*
 * A079785  Number of positions that are exactly n moves from the starting
 *          position in the Beachball puzzle.
 * https://oeis.org/A079785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0)=1, a(1)=8, a(2)=48, a(3)=252, a(4)=930, a(5)=884, a(6)=37,
 *            a(n)=0 for n>6 (distribution of distances in the Beachball
 *            puzzle)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Total positions: 2160, max distance: 6. */
    static const seq_t dist[7] = {1, 8, 48, 252, 930, 884, 37};
    return (n < 7) ? dist[n] : (seq_t)0;
}
