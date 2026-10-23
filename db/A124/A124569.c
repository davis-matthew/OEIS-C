/*
 * A124569  Numbers k such that k, k+2, k+4 and k+6 are squarefree.
 * https://oeis.org/A124569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for odd k such that k, k+2, k+4, k+6 are squarefree. */
#include <stdlib.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n) {
    /* static cache of discovered terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* capacity of the cache */
    static seq_t next_candidate = 1; /* next odd number to test */

    if (n == 0) return 0;  /* offset starts at 1; undefined for 0 */

    /* generate new terms until we have n of them */
    while (cache_len < n) {
        /* search for the next odd k satisfying the condition */
        while (1) {
            seq_t k = next_candidate;
            if (is_squarefree(k) &&
                is_squarefree(k + 2) &&
                is_squarefree(k + 4) &&
                is_squarefree(k + 6)) {
                /* store the found term */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort(); /* allocation failed */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                next_candidate += 2;
                break;
            }
            next_candidate += 2;  /* skip to next odd number */
        }
    }

    return cache[n - 1];
}
