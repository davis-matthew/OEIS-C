/*
 * A178696  Expansion of the polynomial
 *          (1+x^3)*(1+x^5)*(1+x^7)*(1+x^9)*(1+x^11)*(1+x^13)*(1+x^15)*(1+x^17)*(1+x^19).
 * https://oeis.org/A178696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of subsets of {3,5,7,9,11,13,15,17,19} whose sum
 *            is n
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External types: seq_t and idx_t are provided by the build environment. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *coeff = NULL;   /* cache of coefficients, size 100 */
static bool initialized = false;

seq_t a(idx_t n)
{
    /* Lazy initialization of the coefficient cache. */
    if (!initialized) {
        initialized = true;
        coeff = (seq_t *)malloc(100 * sizeof(seq_t));
        if (!coeff) return 0;   /* allocation failure: fall back to 0 */

        /* zero all entries */
        for (idx_t i = 0; i < 100; ++i)
            coeff[i] = 0;

        /* exponent set from the product (1 + x^3)(1 + x^5)...(1 + x^19) */
        const idx_t exps[9] = {3,5,7,9,11,13,15,17,19};

        /* iterate over all 2^9 subsets */
        for (idx_t mask = 0; mask < (1ULL << 9); ++mask) {
            idx_t sum = 0;
            for (idx_t j = 0; j < 9; ++j)
                if (mask & (1ULL << j))
                    sum += exps[j];
            /* sum is always < 100 for the given set */
            coeff[sum] += (seq_t)1;
        }
    }

    /* return the precomputed coefficient if within range, otherwise 0 */
    if (n < 100)
        return coeff[n];
    return 0;
}
