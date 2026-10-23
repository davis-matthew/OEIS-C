/*
 * A170570  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1, a(1) = 33
 * For 2 ≤ n < 47: a(n) = 33 · 32^(n‑1)
 * For n ≥ 47:      a(n) = 496·a(n‑47) – 31·∑_{k=1}^{46} a(n‑k)
 * (Recurrence comes from the rational generating function of the sequence.)
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed terms */
static size_t cache_len = 0;  /* number of terms stored in cache */
static size_t cache_cap = 0;  /* allocation capacity of cache */

/* enlarge the cache if necessary and compute missing terms up to n */
static void ensure_cache(size_t n)
{
    if (n + 1 <= cache_len) return;               /* already present */

    /* extend storage to at least n+1 elements */
    if (cache_cap < n + 1) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < n + 1) new_cap <<= 1;     /* grow geometrically */
        void *p = realloc(cache, new_cap * sizeof(seq_t));
        if (!p) {                                 /* allocation failed */
            /* In the unlikely event of failure we abort; the caller may catch */
            abort();
        }
        cache = (seq_t *)p;
        cache_cap = new_cap;
    }

    /* initialise first term if the cache was empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;      /* a(0) = 1 */
        cache_len = 1;
    }

    /* compute terms from cache_len up to n (inclusive) */
    for (size_t i = cache_len; i <= n; ++i) {
        if (i == 0) continue;   /* already set */

        if (i < 47) {
            /* a(i) = 33 * 32^(i‑1).  For i ≤ 46 the exponent is small. */
            seq_t pow32 = 1;
            for (size_t k = 2; k <= i; ++k) pow32 *= 32;   /* compute 32^(i‑1) */
            cache[i] = (seq_t)33 * pow32;
        } else {
            /* recurrence: a(i) = 496*a(i‑47) – 31*∑_{k=1}^{46} a(i‑k) */
            seq_t sum = 0;
            for (size_t k = 1; k <= 46; ++k) sum += cache[i - k];
            cache[i] = (seq_t)496 * cache[i - 47] - (seq_t)31 * sum;
        }
    }

    cache_len = n + 1;
}

/* external entry point required by the test harness */
seq_t a(idx_t n)
{
    ensure_cache((size_t)n);
    return cache[(size_t)n];
}
