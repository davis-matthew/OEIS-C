/*
 * A153381  Numbers k such that 11*k+5 is not prime.
 * https://oeis.org/A153381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer k such that 11*k+5 is NOT prime
 * Range:     u8 n <= 216, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache of previously found terms, growing as needed. */
    static seq_t *cache       = NULL;   /* a(1)..a(cache_len) */
    static size_t cache_cap   = 0;      /* current malloc'ed capacity */
    static idx_t cache_len    = 0;      /* number of terms stored */
    static idx_t next_k       = 1;      /* next integer to test as k */

    if (n == 0)
        return 0;                       /* offset is 1, ignore n == 0 */

    /* Generate terms until we have at least n terms. */
    while (cache_len < n) {
        /* Find the next k whose 11*k+5 is composite. */
        for (;;) {
            idx_t test = 11 * next_k + 5;    /* 11*k+5 */
            if (!is_prime(test)) {           /* composite detected */
                /* Ensure enough space in the cache. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                   new_cap * sizeof(seq_t));
                    if (!tmp) {               /* allocation failed */
                        /* In production code we might abort, but here we
                           just continue; the loop will retry. */
                        continue;
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_k;  /* store the term */
                ++next_k;                    /* advance to next candidate */
                break;                       /* finished one term */
            }
            ++next_k;                   /* try next integer */
        }
    }

    return cache[n - 1];   /* zero-based indexing into cache */
}
