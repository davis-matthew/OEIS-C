/*
 * A123912  Numbers whose square starts with 2 identical digits.
 * https://oeis.org/A123912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 6641, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A123912: Numbers whose square starts with two identical digits. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *cache      = NULL;   /* array of found terms      */
    static size_t cache_cap  = 0;      /* current capacity         */
    static size_t cache_len  = 0;      /* number of terms stored   */
    static seq_t last_checked = 0;     /* last m that was tested   */

    /* Compute terms until we have at least n of them. */
    while (cache_len < n) {
        /* Next candidate */
        seq_t m = last_checked + 1;

        /* Find the next m that satisfies the property */
        while (true) {
            /* Compute square in a wider type to avoid overflow. */
            unsigned long long sq = (unsigned long long)m * (unsigned long long)m;

            if (sq >= 100ULL) {                    /* at least two digits */
                unsigned long long tmp = sq;
                while (tmp >= 100ULL) {             /* strip trailing digits */
                    tmp /= 10ULL;
                }
                if (tmp % 11ULL == 0ULL) {          /* leading two digits identical */
                    /* store the valid m in the cache */
                    if (cache_len == cache_cap) {   /* grow cache if needed */
                        cache_cap = cache_cap ? cache_cap * 2 : 16;
                        seq_t *tmp_alloc = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                        if (!tmp_alloc) {
                            abort();                     /* allocation failure – terminate */
                        }
                        cache = tmp_alloc;
                    }
                    cache[cache_len++] = (seq_t)m;
                    last_checked = m;              /* remember last tested m */
                    break;                         /* next term will be searched */
                }
            }
            /* advance to the next candidate */
            m++;
        }
    }

    return cache[n - 1];
}
