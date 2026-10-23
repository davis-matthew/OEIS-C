/*
 * A166391  Multiples of 17 whose reversal + 1 is also a multiple of 17.
 * https://oeis.org/A166391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 220, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A166391: Multiples of 17 whose reversal + 1 is also a multiple of 17
   Computed by brute‑force search with memoisation. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/reverse.h"   /* seq_t reverse(seq_t v); */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* not defined for offset 1, but harmless */

    /* Static cache storing the terms found so far. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;        /* next multiple index to test */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t cand = (seq_t)17 * (seq_t)next_k;
        seq_t rev  = reverse(cand);
        /* Compute (rev + 1) % 17 directly. */
        if (((rev + (seq_t)1) % (seq_t)17) == 0) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp)
                    return (seq_t)0;   /* allocation failure – return 0 */
                cache = tmp;
            }
            cache[cache_len++] = cand;
        }
        next_k++;
    }

    return cache[n - 1];
}
