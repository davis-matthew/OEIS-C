/*
 * A203471  a(n) = v(n)/A000178(n), v = A203470, A000178 = (superfactorials).
 * https://oeis.org/A203471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{j=1..n} binom(2*j+1, j+2) (OEIS A203471)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* return a(0) as 0 for safety, but the sequence is defined for n>=1 */
    if (n == 0) return 0;

    seq_t result = 1;
    for (idx_t j = 1; j <= n; ++j) {
        /* binom(2*j+1, j+2) is an integer */
        seq_t term = binomial(2 * j + 1, j + 2);
        result *= term;
    }
    return result;
}
