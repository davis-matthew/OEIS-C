/*
 * A055857  Susceptibility series H_6 for 2-dimensional Ising model (divided
 *          by 2).
 * https://oeis.org/A055857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A055857 defined by the generating function P(x)/Q(x) Q(x) =
 *            (1-x^3)^3 * (1-x)^4 * (1+x)^5 P(x) =
 *            1+x^18+25*(x+x^17)+220*(x^2+x^16)+1149*(x^3+x^15)+
 *            4081*(x^4+x^14)+10768*(x^5+x^13)+22083*(x^6+x^12)+
 *            36283*(x^7+x^11)+48543*(x^8+x^10)+53446*x^9
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 45, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   init_coeffs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/init_coeffs.h"

/* Numerator coefficients P(x) (degree 0 … 18) */
static const seq_t NUM[19] = {
    1, 25, 220, 1149, 4081, 10768, 22083, 36283,
    48543, 53446, 48543, 36283, 22083, 10768, 4081,
    1149, 220, 25, 1
};

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static bool coeffs_initialized = false;
    static int Dcoeff[19];

    if (!coeffs_initialized) {
        init_coeffs(Dcoeff);
        coeffs_initialized = true;
    }

    size_t old_size = cache_size;
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;          /* allocation failed */
        if (cache) {
            if (old_size > 0)
                memcpy(new_cache, cache, old_size * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_size = new_size;
    }

    for (size_t i = old_size; i <= (size_t)n; ++i) {
        seq_t sum = (i < 19) ? NUM[i] : 0;
        for (int k = 1; k <= 18; ++k) {
            if (i >= (size_t)k) {
                int coeff = Dcoeff[k];
                seq_t prod = cache[i - k] * (seq_t)(coeff >= 0 ? coeff : -coeff);
                if (coeff >= 0)
                    sum -= prod;
                else
                    sum += prod;
            }
        }
        cache[i] = sum;
    }

    return cache[(size_t)n];
}
