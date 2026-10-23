/*
 * A176846  Number of iterations of the map n-> n - sigma(bigomega(n)) needed
 *          to reach 1.
 * https://oeis.org/A176846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1138, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, small_sigma
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of iterations of the map n -> n - sigma(bigomega(n))
   needed to reach 1.  The function uses the supplied bigomega() helper
   and a small on‑the‑fly sigma() for the typically very small arguments
   produced by bigomega. */
/* Includes the bigomega utility header, which is part of the provided
   build environment. */
#include "../utils/bigomega.h"
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/small_sigma.h"

seq_t a(idx_t n) {
    /* n must be >= 1.  a(1) = 0 by definition. */
    if (n <= 1) return 0;
    seq_t steps = 0;
    idx_t current = n;
    while (current > 1) {
        seq_t bw = bigomega(current);   /* bigomega returns seq_t */
        /* The value of bigomega for n <= 64-bit is very small (< 100),
           so it safely fits into an idx_t. */
        idx_t bw_int = (idx_t)bw;
        seq_t sigma = small_sigma(bw_int);
        current -= (idx_t)sigma;
        steps++;
    }
    return steps;
}
