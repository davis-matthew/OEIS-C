/*
 * A158620  Partial products of A068601.
 * https://oeis.org/A158620
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{k=2..n} (k^3-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Compute a(n) for n ≥ 2.
 * For n < 2 the value is defined as 0; these indices are not used by OEIS. */
seq_t a(idx_t n) {
    if (n < 2) {
        return 0;          /* 0 is a valid seq_t literal for all numeric types */
    }

    /* --------- static cache of computed values -------------------------------- */
    static seq_t *cache = NULL;     /* array holding a(i) for 0 ≤ i ≤ cache_size-1 */
    static idx_t cache_size = 0;    /* allocated size of the cache array        */
    static idx_t cache_last = 0;    /* highest index that has been computed    */

    /* enlarge the cache if necessary to hold up to index n */
    if ((idx_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) {
            abort();               /* terminate if memory allocation fails */
        }

        /* copy existing cached values, if any */
        if (cache) {
            for (idx_t i = 0; i <= cache_last; ++i) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }

        cache = new_cache;
        cache_size = new_size;
    }

    /* initialise the first term a(2) if it has not yet been stored */
    if (cache_last < 2) {
        cache[2] = (seq_t)7;     /* 2^3 - 1 = 7 */
        cache_last = 2;
    }

    /* compute all terms up to n that are not yet cached */
    for (idx_t k = cache_last + 1; k <= n; ++k) {
        /* compute k^3 - 1 safely in 64‑bit unsigned arithmetic */
        uint64_t kk = (uint64_t)k;
        uint64_t cubed = kk * kk * kk;               /* kk^3 fits in 64 bits for n ≤ 10000 */
        seq_t factor = (seq_t)(cubed - 1ULL);         /* cast to seq_t; works for both
                                                        native integers and arbitrary‑precision */
        cache[k] = cache[k - 1] * factor;            /* use * which is defined for seq_t  */
    }
    cache_last = n;   /* mark that all indices up to n are now computed */

    return cache[n];
}
