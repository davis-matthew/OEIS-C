/*
 * A124430  Eigenvector of triangle A124428.
 * https://oeis.org/A124430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..[n/2]} a(k)*C([n/2],k)*C([(n+1)/2],k) for n>0,
 *            with a(0)=1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 27, u64 n <= 52, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *a_cache = NULL;
static bool *a_computed = NULL;
static size_t a_cache_size = 0;

/* Ensure the cache can hold indices 0..n */
static void ensure_cache(idx_t n) {
    if ((size_t)n >= a_cache_size) {
        size_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        bool *new_computed = (bool *)realloc(a_computed, new_size * sizeof(bool));
        if (!new_cache || !new_computed) {
            exit(EXIT_FAILURE);
        }
        a_cache = new_cache;
        a_computed = new_computed;
        for (size_t i = a_cache_size; i < new_size; ++i) {
            a_computed[i] = false;
        }
        a_cache_size = new_size;
    }
}

seq_t a(idx_t n) {
    ensure_cache(n);
    if (a_computed[n]) {
        return a_cache[n];
    }

    /* Compute values up to n iteratively */
    for (idx_t i = 0; i <= n; ++i) {
        if (a_computed[i]) continue;
        if (i == 0) {
            a_cache[0] = (seq_t)1;
            a_computed[0] = true;
            continue;
        }

        idx_t m = i / 2;
        idx_t t = (i + 1) / 2;
        seq_t sum = 0;
        seq_t binom_m = 1; /* C(m,0) */
        seq_t binom_t = 1; /* C(t,0) */

        for (idx_t k = 0; k <= m; ++k) {
            seq_t term = a_cache[k];
            term *= binom_m;
            term *= binom_t;
            sum += term;

            if (k < m) {
                seq_t tmp = binom_m * (seq_t)(m - k);
                binom_m = tmp / (seq_t)(k + 1);
            }
            if (k < t) {
                seq_t tmp = binom_t * (seq_t)(t - k);
                binom_t = tmp / (seq_t)(k + 1);
            }
        }

        a_cache[i] = sum;
        a_computed[i] = true;
    }

    return a_cache[n];
}
