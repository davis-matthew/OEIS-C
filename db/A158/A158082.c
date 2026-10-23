/*
 * A158082  Squares whose decimal expansion contains no digit greater than 4.
 * https://oeis.org/A158082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 330, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits_le4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158082: Squares whose decimal expansion contains no digit > 4.
   This implementation computes a(n) by searching squares in increasing order
   and keeping only those whose digits are all 0‑4.  The first few terms are:
   0, 1, 4, 100, 121, 144, 324, 400, 441, … (offset 1). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digits_le4.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; a(1) = 0. */
    static seq_t *cache = NULL;   /* dynamic array of found squares   */
    static size_t cap   = 0;      /* capacity of the array           */
    static size_t sz    = 0;      /* number of stored elements        */
    static idx_t  last_i = 0;     /* last i processed (i for i^2)    */

    if (n == 0) return 0; /* undefined; just return 0 */

    /* Ensure the array contains at least n elements. */
    if (n <= sz) return cache[n-1];

    /* If we're starting for the first time, insert the square 0. */
    if (sz == 0) {
        cap = 16;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        cache[0] = 0;
        sz = 1;
        last_i = 0;
    }

    /* Generate squares until we have enough terms. */
    while (sz < (size_t)n) {
        last_i++;                    /* next integer i */
        seq_t v = (seq_t)last_i * (seq_t)last_i;
        if (digits_le4(v)) {
            if (sz >= cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) exit(1);  /* allocation failed */
                cache = tmp;
            }
            cache[sz++] = v;
        }
    }
    return cache[n-1];
}
