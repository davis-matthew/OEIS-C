/*
 * A170059  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the rational generating function P(t)/Q(t) with
 *            P(t) = t^36 + 2*t^35 + … + 2*t + 1 Q(t) = 1 - 48*t - 48*t^2 - …
 *            - 48*t^35 + 1176*t^36 This leads to the recurrence: a(0) = 1
 *            a(n) = P[n] - Σ_{k=1}^{min(n,36)} d[k] * a(n-k) (n ≤ 36) a(n) =
 *            48 * Σ_{k=n-35}^{n-1} a(k) – 1176 * a(n-36)...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for computed terms */
static seq_t *cache      = NULL; /* array of computed a(n) values */
static idx_t  cache_size = 0;    /* number of elements stored (max index+1) */
static size_t cache_cap  = 0;    /* allocated capacity */

/* temporary coefficient tables, filled once */
static int P[37];
static int D[37];
static int tables_initialized = 0;

/* Ensure that cache contains terms up to index n inclusive. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    /* Initialize coefficient tables on first use. */
    if (!tables_initialized) {
        P[0] = 1;
        for (int i = 1; i <= 35; ++i)
            P[i] = 2;
        P[36] = 1;

        D[0] = 1;
        for (int i = 1; i <= 35; ++i)
            D[i] = -48;
        D[36] = 1176;

        tables_initialized = 1;
    }

    /* Resize cache if necessary. */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        /* Grow by factor 2 to reduce reallocations. */
        if (new_cap < 128)
            new_cap = 128;
        else
            while (new_cap < n + 1)
                new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort(); /* critical failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 36) {
            seq_t term = (seq_t)P[(int)i];
            for (idx_t k = 1; k <= i; ++k) {
                int coeff = D[(int)k]; /* may be negative */
                seq_t mult = (seq_t)cache[(size_t)(i - k)]
                           * (seq_t)((coeff > 0) ? coeff : -coeff);
                if (coeff > 0)
                    term -= mult;
                else
                    term += mult;
            }
            cache[(size_t)i] = term;
        } else {
            /* recurrence for n >= 37 */
            seq_t sum35 = 0;
            for (idx_t k = 1; k <= 35; ++k)
                sum35 += cache[(size_t)(i - k)];
            seq_t temp = sum35 - (seq_t)(24) * cache[(size_t)(i - 36)];
            cache[(size_t)i] = (seq_t)(48) * temp;
        }
    }
    cache_size = n + 1;
}

/* Public entry point expected by the build system. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
