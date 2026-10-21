/*
 * A054582  Array read by antidiagonals upwards: A(m,k) = 2^m * (2k+1), m,k >=
 *          0.
 * https://oeis.org/A054582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 5150, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054582: array read by antidiagonals upwards
 * A(m,k) = 2^m * (2k+1)  (m,k ≥ 0)
 * The terms are taken from the infinite array along antidiagonals
 * (s = m + k) in order: (s,0),(s-1,1),...,(0,s).
 * The algorithm finds the diagonal containing n, then the position
 * within that diagonal and returns 2^m * (2k+1).
 */

#include <stddef.h>
#include "../utils/pow2.h"   /* pow2(idx_t) -> seq_t */

seq_t a(idx_t n) {
    /* Find the diagonal sum s = m + k.
       The total number of terms up to diagonal s-1 is
       (s)*(s+1)/2.  We find the smallest s such that
       (s+1)*(s+2)/2 > n.  A simple loop suffices for n up to 10000. */
    idx_t t = 0;                     /* t = s + 1 */
    while ((t * (t + 1)) / 2 <= n) {
        ++t;
    }
    idx_t s   = t - 1;               /* diagonal index */
    idx_t prev = (t - 1) * t / 2;    /* count of terms before this diagonal */
    idx_t pos  = n - prev;           /* position inside the diagonal (0..s) */

    /* Map position within diagonal to (m,k) */
    idx_t m = s - pos;               /* row index */
    idx_t k = pos;                   /* column index */

    /* Compute the term: A(m,k) = 2^m * (2k+1) */
    seq_t term = pow2(m) * (seq_t)(2 * k + 1);
    return term;
}
