/*
 * A170107  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(0) = 1, * a(n) = 48 *
 *            Σ_{i=1}^{min(36,n)} a(n-i) - 1176 * (n≥37 ? a(n-37) : 0) * +
 *            b_n, where b_n = 1 (n=0 or n=37), 2 (1≤n≤36), 0 otherwise. *
 *            This is derived from the generating function * (1 + 2t + … +
 *            2t^36 + t^37)/(1 - 48t - … - 48t^36 + 1176t^37)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>     /* for size_t */
#include <stdlib.h>     /* for malloc/realloc, free */
#include <string.h>     /* for memcpy, memset */

static seq_t *cache = NULL;         /* array holding pre‑computed values */
static idx_t cache_len = 0;         /* number of values stored (cache_len-1 is max index) */
static size_t cache_cap = 0;        /* allocated size of cache */

/* Ensure that cache has space for indices 0 … n (inclusive).  */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;

    size_t new_cap = (size_t)n + 1;
    if (new_cap < 64)
        new_cap = 64;            /* grow in chunks to reduce allocations */

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
#else
    new_cache = malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_cache) { /* allocation failure – abort */
        exit(EXIT_FAILURE);
    }

    /* Copy old contents */
    if (cache_len > 0) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
    }
    /* Initialise new cells to zero */
    memset(new_cache + cache_len, 0,
           (new_cap - cache_len) * sizeof(seq_t));

    /* Free old storage */
#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif

    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n) using memoised recurrence described above. */
seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    ensure_cache(n);

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
            continue;
        }

        /* sum of the previous up to 36 terms */
        seq_t sum = 0;
        idx_t max_prev = (i < 36) ? i : 36;
        for (idx_t j = 1; j <= max_prev; ++j)
            sum += cache[i - j];

        seq_t term = (seq_t)48 * sum;

        if (i >= 37)
            term -= (seq_t)1176 * cache[i - 37];

        /* add the b_n contribution */
        seq_t b = 0;
        if (i == 0 || i == 37)
            b = 1;
        else if (i >= 1 && i <= 36)
            b = 2;

        cache[i] = term + b;
    }

    cache_len = n + 1;
    return cache[n];
}
