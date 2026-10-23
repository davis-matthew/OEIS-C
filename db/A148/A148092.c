/*
 * A148092  The partition function G(n,6).
 * https://oeis.org/A148092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = sum_{j=0}^{min(5,n-1)} binom(n-1,j) * a(n-1-j)
 *   a(0) = 1
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cached_size = 0;

/* Ensure the cache holds values up to index n, computing new entries on demand. */
static void ensure_cache(idx_t n)
{
    if (cache && cached_size > n)
        return; /* already sufficient */

    size_t new_cap = (size_t)(n + 1);
    idx_t old_size = cached_size;  /* number of entries already initialized */

    /* Allocate or grow the cache */
    if (!cache) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!cache)
            return;  /* allocation failed – caller will get garbage, but this should not happen in tests */
    } else if (new_cap > cached_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return;
        cache = tmp;
    }

    /* Compute values for indices from old_size to n inclusive */
    for (idx_t cur = old_size; cur <= n; cur++) {
        if (cur == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        idx_t limit = cur - 1;
        if (limit > 5)
            limit = 5;

        seq_t sum   = (seq_t)0;
        seq_t comb  = (seq_t)1;          /* binomial coefficient (cur-1 choose j) */
        seq_t n_mid = (seq_t)(cur - 1);   /* (cur-1) casted to seq_t */

        for (idx_t j = 0; j <= limit; j++) {
            /* term = a[cur-1-j] * C(cur-1, j) */
            sum += cache[cur - 1 - j] * comb;

            /* update comb for next j (unless j==limit) */
            if (j < limit) {
                comb = comb * (n_mid - j) / (j + 1ULL);
            }
        }
        cache[cur] = sum;
    }

    cached_size = n + 1;
}

/* Sequence function as required by the interface */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
