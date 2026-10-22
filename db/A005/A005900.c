/*
 * A005900  Octahedral numbers: a(n) = n*(2*n^2 + 1)/3.
 * https://oeis.org/A005900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+2,3) + 2*binomial(n+1,3) + binomial(n,3)
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_binom_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_binom_3.h"

seq_t a(idx_t n) {
    seq_t n_cast = (seq_t)n;
    return compute_binom_3(n_cast + 2) + 2 * compute_binom_3(n_cast + 1) + compute_binom_3(n_cast);
}
