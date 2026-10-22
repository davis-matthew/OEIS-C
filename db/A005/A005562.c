/*
 * A005562  Number of walks on square lattice. Column y=4 of A052174.
 * https://oeis.org/A005562
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = C(k+5, ceiling(k/2))*C(k+4, floor(k/2)) - C(k+5,
 *            ceiling((k-1)/2))*C(k+4, floor((k-1)/2)), where k = n-4
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n < 4) return 0;

    idx_t k = n - 4;

    idx_t c1 = (k + 1) / 2;
    idx_t f1 = k / 2;

    seq_t term1 = binom(k + 5, c1) * binom(k + 4, f1);

    seq_t term2;
    if (k == 0) {
        term2 = 0;
    } else {
        idx_t c2 = k / 2;
        idx_t f2 = (k - 1) / 2;
        term2 = binom(k + 5, c2) * binom(k + 4, f2);
    }

    return term1 - term2;
}
