/*
 * A008520  Numbers whose American English name contains the letter 'e'.
 * https://oeis.org/A008520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   A008520. The nth number whose American English name contains the
 *            letter 'e'. The function builds the sequence lazily; it does not
 *            rely on any external tables
 * Range:     u8 n <= 237, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_e
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains_e.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* behaviour below the offset is irrelevant */
        return 0;

    /* lazily built cache of terms */
    static seq_t *cache = NULL;
    static idx_t cache_cnt = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 0;  /* next integer to test */

    /* initialise cache once */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* grow until we have at least n terms */
    while (cache_cnt < n) {
        if (cache_cnt >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        if (contains_e(next_candidate))
            cache[cache_cnt++] = (seq_t)next_candidate;
        next_candidate++;
    }

    return cache[n - 1];
}
