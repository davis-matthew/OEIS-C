/*
 * A055219  T(2n+3,n), array T as in A055216.
 * https://oeis.org/A055219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0}^{⌊n/3⌋} (-1)^j * binomial(n+3, j) *
 *            binomial(2n+3-3j, n+3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t jmax = n / 3;                     /* floor(n/3) */
    idx_t n3 = n + 3;                       /* n+3 */
    for (idx_t j = 0; j <= jmax; ++j) {
        /* binomial(n+3, j) */
        seq_t b1 = binomial(n3, j);
        /* binomial(2n+3-3j, n+3) */
        idx_t big = 2 * n + 3 - 3 * j;      /* 2n+3-3j */
        seq_t b2 = binomial(big, n3);
        seq_t term = b1 * b2;
        if ((j & 1) == 0) {                /* even j: add */
            result += term;
        } else {                           /* odd j: subtract */
            result -= term;
        }
    }
    return result;
}
