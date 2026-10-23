/*
 * A044864  Positive integers having the same number of base-8 runs of odd
 *          length as even.
 * https://oeis.org/A044864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   equal_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A044864: Positive integers having the same number of
   base‑8 runs of odd length as runs of even length.
   This implementation searches for n‑th term by brute force, caching
   results to support out‑of‑order queries.
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/equal_runs.h"

seq_t a(idx_t n)
{
    /* static cache for the sequence terms */
    static seq_t *cache = NULL;   /* array of found terms */
    static size_t have  = 0;      /* number of terms stored */
    static size_t cap   = 0;      /* allocation capacity */
    static seq_t next   = 1;      /* next candidate to test */

    /* initialise cache on first call */
    if (cap == 0) {
        cap = 64;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (have < n) {
        if (equal_runs(next)) {
            if (have == cap) {      /* double the capacity when needed */
                cap *= 2;
                cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
            }
            cache[have++] = next;   /* store the term */
        }
        ++next;                     /* advance to following candidate */
    }

    /* return the nth term (1‑based) */
    return cache[n - 1];
}
