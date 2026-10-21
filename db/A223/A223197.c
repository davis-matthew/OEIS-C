/*
 * A223197  Rolling cube footprints: number of n X 3 0..5 arrays starting with
 *          0 where 0..5 label faces of a cube and every array movement to a
 *          horizontal or vertical neighbor moves across a corresponding cube
 *          edge.
 * https://oeis.org/A223197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 40*a(n-1) - 128*a(n-2), a(1) = 16, a(2) = 576 (OEIS
 *            A223197)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) {
        /* Off‑spec behaviour for n < 1, not used by the suite */
        return 0;
    }

    /* Base cases */
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)576;

    seq_t prev2 = (seq_t)16;   /* a(1) */
    seq_t prev1 = (seq_t)576;  /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = prev1 * (seq_t)40 - prev2 * (seq_t)128;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
