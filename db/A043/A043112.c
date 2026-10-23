/*
 * A043112  Numbers k such that 1 and 4 occur juxtaposed in the base-5
 *          representation of k but not of k-1.
 * https://oeis.org/A043112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A043112 – numbers k such that 1 and 4 occur juxtaposed in
   base‑5 representation of k but not of k‑1 */

#include <stdlib.h>
#include <stdbool.h>

/* Does x contain an adjacent pair of digits 1 and 4 (in either order)
   in its base‑5 representation? */
static bool has14(idx_t x)
{
    int prev = -1;                 /* previous (less significant) digit */
    while (x > 0) {
        int d = (int)(x % 5);      /* current (more significant) digit */
        if ((d == 1 && prev == 4) || (d == 4 && prev == 1))
            return true;
        prev = d;
        x /= 5;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static idx_t cache_size = 0;   /* how many terms we already have          */
    static idx_t next_k = 1;       /* next integer to examine                 */
    static seq_t *cache = NULL;    /* array of found terms                    */
    static size_t cap = 0;         /* capacity of cache                       */

    if (cache == NULL) {           /* initialise cache lazily                 */
        cap   = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort(); /* allocation failure                      */
    }

    /* extend sequence until we have at least n terms             */
    while (cache_size < n) {
        bool cur  = has14(next_k);
        bool prev = has14(next_k - 1);
        if (cur && !prev) {
            if (cache_size >= cap) {      /* grow capacity              */
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (cache == NULL) abort();
            }
            cache[cache_size++] = (seq_t)next_k;
        }
        next_k++;                       /* next integer to test */
    }

    return cache[n - 1];
}
