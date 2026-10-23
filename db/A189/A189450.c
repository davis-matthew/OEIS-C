/*
 * A189450  Number of 2 X n array permutations with each element moving zero
 *          or one space horizontally or diagonally.
 * https://oeis.org/A189450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 4*a(n-3) + a(n-4)  (n ≥ 5)
   Initial terms: a(1)=1, a(2)=5, a(3)=16, a(4)=61 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms (0‑based) */
static idx_t cache_len = 0;      /* number of terms currently stored */
static size_t cache_cap = 0;     /* allocated capacity (in seq_t elements) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out‑of‑range guard */

    /* Allocate or grow the cache so that it can hold n terms */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();
    } else if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        idx_t idx = i - 1;          /* zero‑based index for a(i) */
        if (i == 1) {
            cache[idx] = (seq_t)1;
        } else if (i == 2) {
            cache[idx] = (seq_t)5;
        } else if (i == 3) {
            cache[idx] = (seq_t)16;
        } else if (i == 4) {
            cache[idx] = (seq_t)61;
        } else {
            /* a(i) = 4*a(i-1) - 4*a(i-3) + a(i-4) */
            seq_t t1 = cache[idx-1];
            t1 += cache[idx-1];  /* 2*a(i-1) */
            t1 += cache[idx-1];  /* 3*a(i-1) */
            t1 += cache[idx-1];  /* 4*a(i-1) */

            seq_t t2 = cache[idx-3];
            t2 += cache[idx-3];  /* 2*a(i-3) */
            t2 += cache[idx-3];  /* 3*a(i-3) */
            t2 += cache[idx-3];  /* 4*a(i-3) */

            cache[idx] = t1 - t2 + cache[idx-4];
        }
    }
    cache_len = n;  /* up to a(n) is now stored */

    return cache[n-1];
}
