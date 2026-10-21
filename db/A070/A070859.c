/*
 * A070859  Expansion of (1+x*C^4)*C, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A070859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n, n) + 5*binomial(2*n, n-2)/(n+3) -
 *            binomial(2*n, n-4)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Binomial coefficients are 0 for out‑of‑range k) */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* term binomial(2n, n) */
    seq_t term1 = binomial(2*n, n);

    /* term 5*binomial(2n, n-2)/(n+3) for n>=2 */
    seq_t term2 = 0;
    if (n >= 2) {
        term2 = binomial(2*n, n-2);
        term2 = term2 * 5;
        term2 = term2 / ((seq_t)(n + 3));   /* division is exact */
    }

    /* term -binomial(2n, n-4) for n>=4 */
    seq_t term3 = 0;
    if (n >= 4) {
        term3 = binomial(2*n, n-4);
    }

    return term1 + term2 - term3;
}
