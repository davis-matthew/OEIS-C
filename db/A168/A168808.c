/*
 * A168808  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 33*a(n-1)+...+33*a(n-18) - 561*a(n-19) (n≥20) The first
 *            20 terms are obtained by the ordinary power‑series expansion of
 *            the rational generating function P(t)/Q(t) where P(t) = 1 + 2*t
 *            + 2*t² + … + 2*t¹⁸ + t¹⁹ Q(t) = 1 - 33*t - … - 33*t¹⁸ + 561*t¹⁹
 *            For 1 ≤ n ≤ 18: a(n) = p(n) + 33*Σ_{k=1..n...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   init_coefficients, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

//#include <stdint.h>  // seq_t and idx_t are defined by the test harness
#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/init_coefficients.h"

static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache(idx_t n) {
    // extend storage if necessary
    if (n + 1 > cache_cap) {
        seq_t *new_cache = seq_alloc_array((size_t)n + 1);
        if (cache != NULL) {
            for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = (size_t)n + 1;
    }

    // pre‑compute the coefficient array only once
    static seq_t p_terms[20];
    static int p_inited = 0;
    if (!p_inited) {
        init_coefficients(p_terms);
        p_inited = 1;
    }

    // compute terms up to n
    while (cache_len <= n) {
        seq_t val;
        idx_t i = cache_len;

        if (i == 0) {
            val = p_terms[0];
        } else if (i <= 18) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) sum += cache[i - k];
            val = p_terms[i] + ((seq_t)33) * sum;
        } else if (i == 19) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 18; ++k) sum += cache[19 - k];
            val = p_terms[19] + ((seq_t)33) * sum - ((seq_t)561) * cache[0];
        } else { /* i >= 20 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 18; ++k) sum += cache[i - k];
            val = ((seq_t)33) * sum - ((seq_t)561) * cache[i - 19];
        }

        cache[i] = val;
        ++cache_len;
    }
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
