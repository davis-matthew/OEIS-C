/*
 * A043212  Numbers k such that 5 and 8 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has58
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043212: numbers k such that 5 and 8 occur juxtaposed in the
   base‑9 representation of k but not of k-1.
   The implementation searches successive integers and memoizes found
   terms, handling both orders of the adjacent digits. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has58.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n = 0; placeholder */

    static seq_t *cache = NULL;     /* array of found terms */
    static size_t cache_cap = 0;
    static idx_t found = 0;         /* number of terms stored so far */
    static seq_t next_k = 1;        /* next integer to test */

    /* Grow the cache when needed */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                     /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Find terms until we have the n‑th one */
    while (found < n) {
        if (has58(next_k) && !has58(next_k - (seq_t)1)) {
            cache[found] = next_k;
            found++;
        }
        next_k++;
    }

    return cache[n - 1];
}
