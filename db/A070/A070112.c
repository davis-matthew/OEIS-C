/*
 * A070112  Numbers n such that [A070080(n), A070081(n), A070082(n)] is a
 *          scalene integer triangle.
 * https://oeis.org/A070112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 140, u16 n <= 920, u32 n <= 920, u64 n <= 920,
 *            u128 n <= 920, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the sequence A070112.
 *
 *  A070112 is the list of indices n such that the nth integer triangle
 *  (in the order of all integer triangles sorted by increasing perimeter,
 *   then by first side a, then by second side b) has all three sides
 *  distinct (scalene).
 *
 *  The function enumerates triangles by perimeter and returns the index
 *  of the n‑th scalene triangle.  It uses only integer arithmetic and
 *  is suitable for C11 / C++17.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t scalene_cnt = 0;   /* how many scalene triangles seen  */
    idx_t tri_index   = 0;   /* overall triangle index     */

    for (idx_t p = 3; ; ++p) {             /* perimeter */
        for (idx_t a = 1; a <= p / 3; ++a) { /* smallest side */
            idx_t b_max = (p - a) / 2;      /* b <= c   */
            for (idx_t b = a; b <= b_max; ++b) {
                idx_t c = p - a - b;
                if (a + b <= c) continue;   /* fails triangle inequality */

                ++tri_index;                 /* count this triangle */

                if (a < b && b < c) {       /* scalene triangle */
                    ++scalene_cnt;
                    if (scalene_cnt == n) {
                        return (seq_t)tri_index;
                    }
                }
            }
        }
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
