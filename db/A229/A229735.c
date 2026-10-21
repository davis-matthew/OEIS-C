/*
 * A229735  a(n) = 151*n^7/315+2*n^5/9+7*n^3/45+n/7.
 * https://oeis.org/A229735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,7) + 62*binomial(n+1,7) + 575*binomial(n+2,7)
 *            + 1140*binomial(n+3,7) + 575*binomial(n+4,7) +
 *            62*binomial(n+5,7) + binomial(n+6,7)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 26, u64 n <= 627, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;

    static const idx_t coeff[7] = {1, 62, 575, 1140, 575, 62, 1};
    seq_t result = 0;

    /* Loop over t = 0 .. 6 corresponding to binomial(n+t, 7) */
    for (idx_t t = 0; t < 7; ++t) {
        /* Compute binomial(n + t, 7) only when n + t >= 7 */
        if (n + t < 7) continue;
        seq_t term = binomial(n + t, 7);
        term = term * (seq_t)coeff[t];
        result += term;
    }

    return result;
}
