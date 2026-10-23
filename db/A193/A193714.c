/*
 * A193714  Positions of squares (A000290) in the union of squares and
 *          triangular numbers (A005214).
 * https://oeis.org/A193714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{m : m is a square or a triangular number, m ≤ n²} i.e.
 *            a(n) = ⌊√n²⌋ + ⌊(⌊√(8n²+1)⌋−1)/2⌋ − #{k : k² is both a square
 *            and a triangular number ≤ n²} The last term counts squares that
 *            are also triangular numbers
 * Range:     u8 n <= 5, u16 n <= 422, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_triple_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_triple_squares.h"

static seq_t floor_isqrt(seq_t x) {
    seq_t low = 0, high = x, mid, ans = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid == 0) { ans = 0; break; }
        if (mid <= x / mid) { ans = mid; low = mid + 1; }
        else { high = mid - 1; }
    }
    return ans;
}

seq_t a(idx_t n) {
    /* n ≥ 1 by definition of the sequence */
    /* Count of squares ≤ n² is simply n */
    seq_t squareCnt = (seq_t) n;

    /* Count of triangular numbers ≤ n² */
    seq_t x = (seq_t) n * (seq_t) n;          /* n² */
    seq_t s = floor_isqrt((seq_t)8 * x + 1);  /* floor(√(8n²+1)) */
    seq_t triCnt = (s - 1) >> 1;              /* (s-1)/2 */

    /* Count of overlaps (both square and triangular) */
    idx_t overlapCnt = count_triple_squares(x);

    return squareCnt + triCnt - (seq_t)overlapCnt;
}
