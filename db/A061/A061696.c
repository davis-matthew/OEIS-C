/*
 * A061696  Generalized Bell numbers.
 * https://oeis.org/A061696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!^2 * [x^n] exp(BesselI(0,2*sqrt(x)) - 1 - x)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (for all n ≥ 1):                              */
/*   a(n) = (1/n) * ∑_{k=2}^{n} k * C(n,k)^2 * a(n-k)       */
/* with a(0) = 1 and a(1) = 0.                              */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *alloc_cache(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  len   = 0;

    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache) {
            cache[0] = (seq_t)1;
            cap = 1;
            len = 0;
        }
    }

    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return NULL;
        cache = tmp;
        cap   = new_cap;
    }

    for (idx_t m = len + 1; m <= n; ++m) {
        /* use 128‑bit intermediates when seq_t is ≤ 8 bytes */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 sum   = 0;
            unsigned __int128 binom = 1;  /* binom(m,0) */

            for (idx_t k = 1; k <= m; ++k) {
                binom = binom * (m - k + 1) / k;

                if (k >= 2) {
                    unsigned __int128 term = binom * binom;
                    term *= k;
                    term *= (unsigned __int128)cache[m - k];
                    sum += term;
                }
            }
            cache[m] = (seq_t)(sum / m);
        } else { /* seq_t is an arbitrary‑precision type */
            seq_t sum   = 0;
            seq_t binom = 1; /* binom(m,0) */

            for (idx_t k = 1; k <= m; ++k) {
                binom = binom * (seq_t)(m - k + 1) / k;

                if (k >= 2) {
                    seq_t term = binom * binom;
                    term = term * k;
                    term = term * cache[m - k];
                    sum += term;
                }
            }
            cache[m] = sum / m;
        }
    }
    len = (len > n) ? len : n;
    return cache;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    cache = alloc_cache(n);
    if (!cache) return 0;
    return cache[n];
}
