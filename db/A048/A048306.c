/*
 * A048306  Numbers whose base-9 expansions, read from left to right, have run
 *          lengths that strictly increase.
 * https://oeis.org/A048306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth (0‑indexed) integer whose base‑9 expansion has
 *            strictly increasing run lengths. The sequence is computed by a
 *            simple brute‑force search
 * Range:     u8 n <= 36, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   increasing_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/increasing_run.h"

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;
    static idx_t   count     = 0;          /* how many terms cached */
    static idx_t   next      = 0;          /* next integer to test  */

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n+1 of them */
    while (count <= n) {
        if (increasing_run((seq_t)next)) {
            if (count == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[count++] = (seq_t)next;
        }
        ++next;
    }

    return cache[n];
}
