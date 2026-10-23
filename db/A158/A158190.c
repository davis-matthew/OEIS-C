/*
 * A158190  Nonprime numbers with final digit prime.
 * https://oeis.org/A158190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A158190: Nonprime numbers whose last digit is a prime digit (2,3,5,7).
   a(1) = 12, a(2) = 15, …  a(n) is the n‑th composite integer ending in 2,3,5 or 7. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */
    static idx_t cache_cap = 0;   /* allocated capacity */
    static idx_t last_checked = 0; /* last integer examined */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – safety return */
    }

    /* generate terms until we have the nth one */
    while (cache_len < n) {
        /* find next integer with a prime last digit */
        do {
            ++last_checked;
        } while (last_checked % 10 != 2 &&
                 last_checked % 10 != 3 &&
                 last_checked % 10 != 5 &&
                 last_checked % 10 != 7);

        /* skip primes */
        if (!is_prime(last_checked)) {
            /* ensure enough capacity */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    cache = NULL;
                    return 0; /* allocation failure */
                }
                cache = tmp;
            }
            /* store the term */
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1]; /* a(1) is stored at cache[0] */
}
