/*
 * A005566  Number of walks of length n on square lattice, starting at origin,
 *          staying in first quadrant.
 * https://oeis.org/A005566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = binomial(n, floor(n/2))*binomial(n+1, floor((n+1)/2))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    idx_t k1 = n / 2;
    idx_t k2 = (n + 1) / 2;
    seq_t b1 = binom(n, k1);
    seq_t b2 = binom(n + 1, k2);
    return b1 * b2;
}
