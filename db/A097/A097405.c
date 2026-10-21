/*
 * A097405  Number of different rectangles created when a square sheet of
 *          paper is folded n times, the first time by one of the diagonals of
 *          the square and after by the median of the triangle.
 * https://oeis.org/A097405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -1 + p(2^ceil(n/2-1)) + d(2^floor(n/2)) where p(x) = x^2
 *            (x+1)^2/4 and d(x) = (x^4 - x^2 - 6x)/24. a(1) = 0, a(2) = 0
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n <= 2) return 0;

    idx_t m1 = (n & 1) ? (n - 1) / 2 : n / 2 - 1; /* ceil(n/2-1) */
    idx_t m2 = n / 2;                              /* floor(n/2)   */

    seq_t x1 = pow2(m1); /* 2^ceil(n/2-1) */
    seq_t x2 = pow2(m2); /* 2^floor(n/2)   */

    /* compute p(x1) = ((x1*(x1+1))/2)^2 */
    seq_t tmp = x1 * (x1 + 1);
    tmp /= 2;
    seq_t p_val = tmp * tmp;

    /* compute d(x2) = (x2^4 - x2^2 - 6*x2)/24 */
    seq_t x2sq = x2 * x2;            /* x2^2 */
    seq_t x2pow4 = x2sq * x2sq;      /* x2^4 */
    seq_t d_num = x2pow4 - x2sq - (x2 * 6);
    seq_t d_val = d_num / 24;

    seq_t res = p_val + d_val;
    res -= 1;
    return res;
}
