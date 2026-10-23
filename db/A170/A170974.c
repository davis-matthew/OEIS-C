/*
 * A170974  Expansion of Product_{i=0..m-1} (1 + x^(4*i+1)) for m = 11.
 * https://oeis.org/A170974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = coefficient of x^n in ∏_{i=0}^{10} (1 + x^(4*i+1)) The
 *            polynomial has degree 231 and is symmetric: a(n) = a(231-n)
 * Range:     u8 n <= 231, u16 n <= 231, u32 n <= 231, u64 n <= 231,
 *            u128 n <= 231, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const idx_t EXPS[11] = {1,5,9,13,17,21,25,29,33,37,41};
static const idx_t MAX_DEG   = 231;          /* 1+5+...+41 */
static const idx_t ARRAY_SIZE = MAX_DEG + 1; /* 232 */

seq_t a(idx_t n)
{
    /* Static cache of coefficients, built once */
    static seq_t *coeff = NULL;
    if (!coeff) {
        coeff = (seq_t *)malloc(ARRAY_SIZE * sizeof(seq_t));
        if (!coeff) return 0;              /* allocation failure, return 0 */

        /* initialise to zero */
        for (idx_t i = 0; i < ARRAY_SIZE; ++i) coeff[i] = 0;

        coeff[0] = 1; /* start with polynomial 1 */
        for (int e = 0; e < 11; ++e) {
            /* multiply current polynomial by (1 + x^EXPS[e]) */
            idx_t exp = EXPS[e];
            for (idx_t j = ARRAY_SIZE; j-- > exp; ) {
                coeff[j] += coeff[j - exp];
            }
        }
    }

    if (n >= ARRAY_SIZE) return 0;
    return coeff[n];
}
