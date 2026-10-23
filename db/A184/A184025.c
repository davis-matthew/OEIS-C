/*
 * A184025  1/8 the number of (n+1)X6 0..3 arrays with all 2X2 subblock sums
 *          the same.
 * https://oeis.org/A184025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 15, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A184025: 1/8 the number of (n+1)X6 0..3 arrays with all 2X2 subblock sums the same
   Empirical recurrence (15 terms):
   a(n) = 8*a(n‑1) + 16*a(n‑2) – 268*a(n‑3) + 189*a(n‑4) + 3388*a(n‑5) – 5824*a(n‑6)
          – 20048*a(n‑7) + 49756*a(n‑8) + 52752*a(n‑9) – 196896*a(n‑10)
          – 24192*a(n‑11) + 369216*a(n‑12) – 131328*a(n‑13) – 262656*a(n‑14)
          + 165888*a(n‑15)
   Initial values a(1) … a(15) are given below. */

#include <stdlib.h>   /* malloc, realloc, free, exit */

static const seq_t init_vals[15] = {
    2312ULL, 3781ULL, 6650ULL, 13603ULL, 29432ULL,
    70867ULL, 177386ULL, 477919ULL, 1325792ULL, 3881491ULL,
    11637650ULL, 36399823ULL, 116243192ULL, 384453427ULL,
    1295419706ULL
};

static seq_t *cache   = NULL;   /* cached terms, 1‑based index → cache[n‑1]   */
static size_t cache_sz = 0;     /* number of elements currently cached     */

/* Ensure that the cache array contains at least n terms (n ≥ 1). */
static void ensure_cache_size(idx_t n)
{
    if (cache_sz >= (size_t)n)
        return;                          /* already enough terms */

    size_t new_size = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – terminate the program. */
        exit(EXIT_FAILURE);
    }
    cache = tmp;

    /* Initialise first 15 terms if not already done.  The loop copies
       only the needed number of elements (up to new_size). */
    if (cache_sz < 15) {
        size_t upto = new_size < 15 ? new_size : 15;
        for (size_t i = cache_sz; i < upto; ++i)
            cache[i] = init_vals[i];
    }

    /* Compute further terms using the empirical recurrence. */
    if (new_size > 15) {
        /* Start computing from the smallest index that requires the recurrence. */
        size_t start = cache_sz > 15 ? cache_sz + 1 : 16;
        if (start < 16) start = 16;      /* safety for small cache_sz */

        for (size_t t = start; t <= new_size; ++t) {
            /* t is the term index (1‑based); cache[t-1] will hold a(t). */
            seq_t pos = 0, neg = 0;

            pos +=  8ULL   * cache[t - 2];
            pos += 16ULL   * cache[t - 3];
            neg += 268ULL  * cache[t - 4];
            pos += 189ULL  * cache[t - 5];
            pos += 3388ULL * cache[t - 6];
            neg += 5824ULL * cache[t - 7];
            neg += 20048ULL* cache[t - 8];
            pos += 49756ULL* cache[t - 9];
            pos += 52752ULL* cache[t -10];
            neg += 196896ULL*cache[t -11];
            neg += 24192ULL *cache[t -12];
            pos += 369216ULL*cache[t -13];
            neg += 131328ULL*cache[t -14];
            neg += 262656ULL*cache[t -15];
            pos += 165888ULL*cache[t -16];

            cache[t - 1] = pos - neg;
        }
    }

    cache_sz = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n < 1; behaviour not required */
        return 0;

    ensure_cache_size(n);
    return cache[n - 1];
}
