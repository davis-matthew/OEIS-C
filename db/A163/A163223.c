/*
 * A163223  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 38*a(n-1) + 38*a(n-2) + 38*a(n-3) - 741*a(n-4) Initial
 *            values: a(0)=1, a(1)=40, a(2)=1560, a(3)=60840, a(4)=2371980
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of A163223.  The function is memoised in a
   static cache, so calls in any order are O(n) time in total. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of terms currently stored */

    /* If we already have the answer, return it. */
    if (n < cache_size) {
        return cache[n];
    }

    /* Expand the cache up to n (inclusive). */
    idx_t old_size   = cache_size;
    idx_t new_size   = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {                      /* out‑of‑memory – return 0 (rare) */
        return (seq_t)0;
    }
    cache = tmp;

    /* Initialise the known first five terms (if the cache is large enough). */
    if (new_size > 0) cache[0] = (seq_t)1;
    if (new_size > 1) cache[1] = (seq_t)40;
    if (new_size > 2) cache[2] = (seq_t)1560;
    if (new_size > 3) cache[3] = (seq_t)60840;
    if (new_size > 4) cache[4] = (seq_t)2371980;

    /* Compute new terms using the recurrence.  Start at the first index
       that really needs the recurrence: 5.  If the cache already stored
       terms beyond that, we resume from the old size. */
    idx_t start = (old_size > 5) ? old_size : 5;
    for (idx_t k = start; k <= n; ++k) {
        /* All operations are performed in seq_t to avoid overflow of
           intermediate 32‑bit ints. */
        seq_t t1 = (seq_t)38 * cache[k-1];
        seq_t t2 = (seq_t)38 * cache[k-2];
        seq_t t3 = (seq_t)38 * cache[k-3];
        seq_t t4 = (seq_t)741 * cache[k-4];
        cache[k] = t1 + t2 + t3 - t4;
    }

    cache_size = new_size;
    return cache[n];
}
