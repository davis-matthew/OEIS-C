/*
 * A143961  Binomial transform of A010054 (characteristic function of
 *          triangular numbers).
 * https://oeis.org/A143961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * A010054(k)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A010054(k) = 1 if k is a triangular number, 0 otherwise. */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;   /* memoized a(n) values */
static size_t cache_cap = 0;  /* allocated size of cache */
static size_t cache_size = 0; /* number of computed values (max index + 1) */

static seq_t *comb = NULL;    /* current row of Pascal’s triangle */
static size_t comb_cap = 0;   /* allocated size of comb array */

/* Helper to ensure that an array has a given capacity, zero‑initialising
   any newly allocated elements. */
static void ensure_capacity(size_t *cap, seq_t **arr, size_t needed)
{
    if (*cap < needed) {
        seq_t *new_arr = (seq_t*)realloc(*arr, sizeof(seq_t)*needed);
        if (!new_arr) abort();                  /* out of memory */
        if (needed > *cap)
            memset(&new_arr[*cap], 0, sizeof(seq_t)*(needed-*cap));
        *arr = new_arr;
        *cap = needed;
    }
}

seq_t a(idx_t n)
{
    /* Initialise structures if this is the first call. */
    if (cache == NULL) {
        ensure_capacity(&cache_cap, &cache, 1);
        ensure_capacity(&comb_cap, &comb, 1);
        comb[0] = 1;        /* row 0 of Pascal’s triangle */
        cache[0] = 1;       /* a(0) = 1 (0 is triangular) */
        cache_size = 1;     /* computed up to index 0 */
    }

    /* Compute values up to n if necessary. */
    for (idx_t cur = cache_size; cur <= n; ++cur) {
        /* Prepare comb array for the new row cur. */
        ensure_capacity(&comb_cap, &comb, (size_t)cur + 1);

        /* Update comb[k] = comb[k] + comb[k-1] for k = cur .. 1 */
        for (idx_t k = cur; k >= 1; --k)
            comb[k] = comb[k] + comb[k-1];
        comb[0] = 1;   /* C(cur,0) = 1 */

        /* Sum binomial coefficients at triangular indices. */
        seq_t s = 0;
        for (idx_t j = 0;; ++j) {
            idx_t t = j * (j + 1) / 2;   /* j-th triangular number */
            if (t > cur) break;
            s += comb[t];
        }

        /* Store result in cache. */
        ensure_capacity(&cache_cap, &cache, (size_t)cur + 1);
        cache[cur] = s;
    }

    cache_size = (size_t)n + 1;  /* we now have values up to n */

    return cache[n];
}
