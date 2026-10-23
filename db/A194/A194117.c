/*
 * A194117  a(n) = Sum_{j=1..n} floor(j*(1+sqrt(3))/2); n-th partial sum of
 *          Beatty sequence A003511.
 * https://oeis.org/A194117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} floor(j*(1+sqrt(3))/2)
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Beatty term floor(j*(1+sqrt(3))/2) can be computed using
   integer arithmetic as floor((j + floor(j*sqrt(3)))/2).  Here
   floor(j*sqrt(3)) is obtained by floor_sqrt(3*j*j).  The function
   a(n) computes the partial sum in O(n) time. */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j) {
        seq_t jj = (seq_t)j;
        seq_t prod = (seq_t)3 * jj * jj;
        seq_t sqrt_term = floor_sqrt(prod);
        seq_t beat = (jj + sqrt_term) / 2;
        sum += beat;
    }
    return sum;
}
