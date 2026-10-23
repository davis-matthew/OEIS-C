/*
 * A039448  Numbers whose base-11 representation has the same nonzero number
 *          of 1's and 2's.
 * https://oeis.org/A039448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Numbers whose base‑11 representation has the same non‑zero *
 *            number of 1’s and 2’s. Search for the n‑th such integer
 *            (a(1)=13)
 * Range:     u8 n <= 22, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Return the n‑th term of sequence A039448.
   n is 1‑based; a(1) = 13. */
seq_t a(idx_t n)
{
    /* memoisation buffer -------------------------------------------- */
    static seq_t *cache    = NULL;   /* array of found terms          */
    static size_t cache_len = 0;    /* number of terms stored        */
    static size_t cache_cap = 0;    /* current allocation size       */
    static uint64_t last_checked = 0; /* last integer examined        */

    if (n == 0)                          /* offset is 1; behaviour below */
        return 0;                       /* is undefined                 */

    /* grow the cache if required ----------------------------------- */
    if (cache_len < (size_t)n) {
        if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap ? cache_cap * 2 : n;
            /* cast the void * from realloc for C++ compatibility   */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* iterate until we have found n terms ----------------------- */
        for (;;) {
            ++last_checked;                     /* next decimal number */

            uint64_t v = last_checked;
            unsigned c1 = 0, c2 = 0;
            while (v > 0) {                     /* base‑11 digits     */
                unsigned d = (unsigned)(v % 11);
                if (d == 1) ++c1;
                else if (d == 2) ++c2;
                v /= 11;
            }
            if (c1 == c2 && c1 > 0) {           /* equal non‑zero counts */
                cache[cache_len++] = (seq_t)last_checked;
                if (cache_len == (size_t)n)
                    break;                     /* enough terms found */
            }
        }
    }
    return cache[n - 1];                     /* 1‑based indexing   */
}
