/*
 * A194144  Sum{floor(j*(-1+sqrt(5))) : 1<=j<=n}; n-th partial sum of Beatty
 *          sequence for -1+sqrt(5).
 * https://oeis.org/A194144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{j=1}^n floor(j*(-1+sqrt(5)))
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here floor(j*(-1+sqrt(5))) = floor(j*sqrt(5)) - j, using
   floor_sqrt(5*j*j) to compute floor(j*sqrt(5)) exactly. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j) {
        seq_t jj = (seq_t)j;
        seq_t s = floor_sqrt((seq_t)5 * jj * jj);
        sum += s - jj;
    }
    return sum;
}
