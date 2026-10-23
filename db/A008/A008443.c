/*
 * A008443  Number of ordered ways of writing n as the sum of 3 triangular
 *          numbers.
 * https://oeis.org/A008443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look, nice
 * Formula:   a(n) = number of ordered triples of odd positive integers (y1,
 *            y2, y3) with y1² + y2² + y3² = 8·n + 3. This is equivalent to
 *            the number of ordered ways of writing n as the sum of three
 *            triangular numbers T_k = k(k+1)/2
 * Range:     u8 n <= 7, u16 n <= 1782, u32 n <= 8609, u64 n <= 8609,
 *            u128 n <= 8609, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* 8*n + 3 fits in seq_t (any size) */
    seq_t S = (seq_t)n * (seq_t)8 + (seq_t)3;

    /* maximum odd y <= sqrt(S) */
    idx_t max_y = (idx_t)floor_sqrt(S);

    seq_t count = 0;

    for (idx_t y1 = 1; y1 <= max_y; y1 += 2) {
        seq_t y1sq = (seq_t)y1 * (seq_t)y1;
        if (y1sq > S) break;
        for (idx_t y2 = 1; y2 <= max_y; y2 += 2) {
            seq_t y2sq = (seq_t)y2 * (seq_t)y2;
            seq_t sum12 = y1sq + y2sq;
            if (sum12 > S) break;
            seq_t rem = S - sum12;
            seq_t y3 = floor_sqrt(rem);
            /* y3 must be odd and positive */
            if (y3 * y3 == rem && (y3 & 1)) {
                count++;
            }
        }
    }

    return count;
}
