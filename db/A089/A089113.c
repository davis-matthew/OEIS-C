/*
 * A089113  Convoluted convolved Fibonacci numbers G_7^(r).
 * https://oeis.org/A089113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=13, a(2)=34, a(3)=77, a(4)=146, a(5)=259, a(6)=418, *
 *            a(7)=654, a(8)=967, a(9)=1396, a(10)=1946, a(11)=2665, *
 *            a(12)=3555, a(13)=4683, a(14)=6048, a(15)=7728. * Recurrence
 *            (order 15): * * a(n) = 2·a(n‑1) + 1·a(n‑2) – 3·a(n‑3) – 1·a(n‑4)
 *            + 1·a(n‑5) * + 4·a(n‑6) – 3·a(n‑7) – 3·a(n‑8) + 4·a(n‑9...
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */

/* Initial 15 terms (index 0 unused). */
static const seq_t init[16] = {
    0ULL,   /* a(0) – unused, kept 0 for safety */
    13ULL, 34ULL, 77ULL, 146ULL, 259ULL, 418ULL, 654ULL, 967ULL,
    1396ULL, 1946ULL, 2665ULL, 3555ULL, 4683ULL, 6048ULL, 7728ULL
};

/* Coefficients of the recurrence (index 0 unused). */
static const int coef[16] = {
    0,  2,  1, -3, -1,  1,  4, -3, -3,  4,  1, -1, -3,  1,  2, -1
};

static seq_t *cache      = NULL;  /* cached values a(0)..a(n) */
static size_t cache_cap  = 0;      /* allocated size of cache */
static idx_t  last_computed = 0;   /* highest index whose value is known */

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)(n + 1);                /* include a(0) */
    if (need > cache_cap) {                      /* allocate or grow   */
        seq_t *tmp = (seq_t *)malloc(need * sizeof(seq_t));
        if (!tmp) abort();                       /* allocation failed */
        if (cache) {                             /* copy old data      */
            for (size_t i = 0; i < cache_cap && i <= (size_t)last_computed; ++i)
                tmp[i] = cache[i];
            free(cache);
        }
        cache     = tmp;
        cache_cap = need;
    }

    /* Initialise the first 15 terms if they are not yet in the cache. */
    if (last_computed < 15) {
        size_t limit = (size_t)((n < 15) ? n : 15);
        for (size_t i = last_computed + 1; i <= limit; ++i)
            cache[i] = init[i];
        if (limit > last_computed)
            last_computed = (idx_t)limit;
    }

    /* Compute whatever new terms are required. */
    if (n >= 16 && last_computed < n) {
        for (idx_t i = (last_computed < 16 ? 16 : last_computed + 1); i <= n; ++i) {
            seq_t pos = 0, neg = 0;
            for (int k = 1; k <= 15; ++k) {
                seq_t v = cache[(size_t)(i - k)];
                int c   = coef[k];
                if (c > 0)
                    pos += v * (seq_t)c;
                else if (c < 0)
                    neg += v * (seq_t)(-c);
            }
            cache[(size_t)i] = pos - neg;
        }
        last_computed = n;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* safety for offset 1   */
    if (!cache || cache_cap <= (size_t)n)      /* ensure cache ready     */
        ensure_cache(n);
    return cache[(size_t)n];
}
