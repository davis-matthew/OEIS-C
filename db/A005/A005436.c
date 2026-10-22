/*
 * A005436  Number of convex polygons of perimeter 2n on square lattice.
 * https://oeis.org/A005436
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n + 3)*4^(n-4) - 4*(n-3)*C(2*n-7, n-4) for n >= 4;
 *            a(2) = 1, a(3) = 2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   a005436_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a005436_binom.h"

seq_t a(idx_t n) {
    seq_t pow4 = 1;
    seq_t term1, term2;
    idx_t i;
    if (n == 2) return 1;
    if (n == 3) return 2;

    for (i = 0; i < n - 4; i++) pow4 = pow4 * 4;

    term1 = (seq_t)(2 * n + 3) * pow4;
    term2 = (seq_t)4 * (seq_t)(n - 3) * a005436_binom(2 * n - 7, n - 4);

    return term1 - term2;
}
