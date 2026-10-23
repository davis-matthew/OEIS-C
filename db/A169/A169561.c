/*
 * A169561  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A169561: Number of reduced words of length n in the Coxeter
   group on 20 generators with relations S_i^2 = (S_i S_j)^35 = I.
   The sequence satisfies the linear recurrence
       a(0) = 1
       a(n) = 2 + 18 * Σ_{k=1}^{34} a(n-k)          for 1 ≤ n ≤ 34
       a(35) = 1 + 18 * Σ_{k=1}^{34} a(35-k) – 171*a(0)
       a(n) = 18 * Σ_{k=1}^{34} a(n-k) – 171*a(n–35)  for n ≥ 36
   which follows from the generating function
       (t^35 + 2t^34 + … + 2t + 1) / (1 – 18t – … – 18t^34 + 171t^35).
   The function below memoises successive terms, allowing arbitrary
   ordering of calls and runs in O(n) time for n up to about 10⁴. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* memoisation buffer */
static size_t cache_cap = 0;   /* capacity of buffer */
static idx_t  cache_len = 0;   /* number of values already computed */

/* Grow the cache so it can hold at least new_cap elements. */
static int grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return 0;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { free(cache); cache = NULL; cache_cap = 0; return -1; }
    cache = tmp;  cache_cap = new_cap;  return 0;
}

seq_t a(idx_t n)
{
    if (grow_cache(n + 1) < 0) return (seq_t)0;   /* out of memory */

    while (cache_len <= n) {
        idx_t i = cache_len;
        seq_t val;

        if (i == 0) {                        /* a(0) */
            val = (seq_t)1;
        } else if (i <= 34) {                /* 1 ≤ n ≤ 34 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 34; ++k)
                if (i >= k) sum += cache[i - k];
            val = 2 + 18 * sum;
        } else if (i == 35) {                /* a(35) */
            seq_t sum = 0;
            for (idx_t j = 1; j <= 34; ++j) sum += cache[j];
            val = 1 + 18 * sum - 171 * cache[0];
        } else {                             /* n ≥ 36 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 34; ++k)
                sum += cache[i - k];
            val = 18 * sum - 171 * cache[i - 35];
        }

        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[n];
}
