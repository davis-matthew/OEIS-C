/*
 * A169272  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 19 * 18^(n-1) for 1 ≤ n ≤ 28 a(n) =
 *            17*a(n-1)+17*a(n-2)+...+17*a(n-28)-153*a(n-29) for n ≥ 29
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow18
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes the sequence on demand and memoises
   all previously computed values.  The recurrence follows from the
   generating function given in the OEIS entry. */

#include <stdlib.h>          /* for malloc/realloc */
#include "../utils/pow18.h"

/* static cache for memoisation */
static seq_t *cache   = NULL;
static idx_t cache_len = (idx_t)0;   /* number of elements stored   */
static size_t cache_cap = 0;         /* allocated capacity */

/* enlarge the cache if necessary */
static void ensure_capacity(idx_t upto) {
    if ((size_t)(upto + 1) <= cache_cap) return;
    size_t newcap = (size_t)(upto + 1) * 2;
    seq_t *p = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    /* If realloc fails we simply leave cache as NULL; the caller
       will never receive a value for that n. */
    cache = p;
    cache_cap = newcap;
}

/* main accessor */
seq_t a(idx_t n) {
    /* extend cache if needed */
    if (n + (idx_t)1 > cache_len) {
        ensure_capacity(n);
        /* compute sum of the last up to 28 terms before n */
        seq_t sum28 = (seq_t)0;
        /* summation of cache[cache_len-1], cache[cache_len-2], ..., cache[max(0, cache_len-28)] */
        for (idx_t k = 0; k < 28 && cache_len > k; ++k) {
            sum28 += cache[cache_len - 1 - k];
        }

        /* fill new entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == (idx_t)0) {
                cache[i] = (seq_t)1;
            } else if (i <= (idx_t)28) {
                cache[i] = (seq_t)19 * pow18(i - (idx_t)1);
            } else {
                /* recurrence: a[n] = 17*sum28 - 153*a[n-29] */
                cache[i] = (seq_t)17 * sum28 - (seq_t)153 * cache[i - (idx_t)29];
            }
            /* update sliding window sum28 */
            sum28 += cache[i];
            if (i >= (idx_t)28) {
                sum28 -= cache[i - (idx_t)28];
            }
        }
        cache_len = n + (idx_t)1;
    }
    return cache[n];
}
