/*
 * A043399  Numbers whose base-7 representation contains exactly three 1's.
 * https://oeis.org/A043399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base-7 representation contains exactly three 1's.
   The implementation scans integers, counts the 1's in base 7, and memoises
   results so repeated calls are fast. */
#include <stdlib.h>
#include <stdbool.h>

/* Return the n-th term (1‑based) of A043399.  */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;    /* array of terms */
    static size_t  cap   = 0;      /* allocated capacity */
    static size_t  len   = 0;      /* number of values stored */
    static idx_t  next_start = 1;  /* next integer to consider */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) /* fallback: abort on allocation failure */
            exit(EXIT_FAILURE);
    }

    /* compute up to the required n */
    while (len < n) {
        /* find the next integer whose base‑7 representation has exactly
           three 1's */
        while (true) {
            seq_t val = (seq_t)next_start;
            next_start++;                     /* advance for next trial */

            /* count number of 1's in base 7 */
            seq_t t = val;
            int ones = 0;
            while (t > (seq_t)0) {
                if ((t % (seq_t)7) == (seq_t)1)
                    if (++ones > 3) break;    /* early exit */
                t /= (seq_t)7;
            }
            if (ones == 3) {
                /* store the term */
                if (len == cap) {
                    cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                    if (tmp == NULL)
                        exit(EXIT_FAILURE);
                    cache = tmp;
                }
                cache[len++] = val;
                break;                           /* found a term, go on */
            }
        }
    }

    return cache[n - 1];
}
