/*
 * A189935  a(n) = A189933(n)/2.
 * https://oeis.org/A189935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n - floor(2*n*sqrt5) - 1
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 7206, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here floor(k*sqrt5) is computed as floor_sqrt(5*k*k) */

/* Include the utility for integer square root */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;          /* convert index to seq_t */
    seq_t k = 2 * N;             /* k = 2*n   */
    /* floor(k*sqrt5) = floor_sqrt(5*k*k) */
    seq_t sub = floor_sqrt((seq_t)5 * k * k);
    seq_t res = 6 * N - sub - 1;
    return res;
}
