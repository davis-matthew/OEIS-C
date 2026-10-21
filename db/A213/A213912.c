/*
 * A213912  a(1) = 1; for n>1, a(n) = floor(sqrt(a(n-1))) if that number is
 *          not already in the sequence, otherwise a(n) = 3*a(n-1).
 * https://oeis.org/A213912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 60, u32 n <= 7728, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n>1, a(n) = floor_sqrt(a(n-1)) if that number
   is not already in the sequence, otherwise a(n) = 3*a(n-1). */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    if (n == 0)                           /* undefined; return 0 for safety */
        return (seq_t)0;

    /* initialise first term if cache empty */
    if (cache_size == 0) {
        if (cache_cap == 0) {
            cache_cap = 8;
            cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
            if (!cache) abort();          /* out of memory */
        }
        cache[0] = (seq_t)1;              /* a(1) = 1 */
        cache_size = 1;
    }

    /* extend cache up to index n */
    while (cache_size < (size_t)n) {
        /* grow storage if necessary */
        if (cache_cap < cache_size + 1) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        seq_t prev = cache[cache_size - 1];
        seq_t cand = floor_sqrt(prev);

        bool exists = false;
        for (size_t i = 0; i < cache_size; ++i) {
            if (cache[i] == cand) {
                exists = true;
                break;
            }
        }

        seq_t next = exists ? prev * (seq_t)3 : cand;
        cache[cache_size] = next;
        ++cache_size;
    }

    return cache[n - 1];
}
