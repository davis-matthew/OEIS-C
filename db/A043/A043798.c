/*
 * A043798  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 0 mod 8.
 * https://oeis.org/A043798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find numbers whose base‑3 run count is congruent to 0 (mod 8) by
   enumerating integers and testing the run condition. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static state to store already found terms */
    static seq_t        *terms   = NULL;   /* array of found numbers */
    static size_t       length  = 0;       /* number of terms stored   */
    static size_t       capacity= 0;       /* allocated capacity       */
    static seq_t        next    = 0;       /* smallest integer not yet
                                              examined                 */

    /* initialise the cache on first call */
    if (terms == NULL) {
        capacity = 1024;
        terms = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (terms == NULL) return 0;   /* allocation failed */
    }

    /* extend the cache until it contains at least n terms */
    while (length < (size_t)n) {
        seq_t k = next + 1;            /* candidate integer */
        seq_t x = k;
        int   runs = 0;                /* number of runs in base 3 */
        seq_t prev_digit = 0;
        int   first = 1;               /* flag for first digit */

        /* base‑3 representation processing */
        if (x == 0) {                  /* handled only if k==0; not needed
                                         * as we start from 1 but kept for
                                         * completeness                               */
            runs = 1;
        } else {
            while (x > 0) {
                seq_t d = x % 3;
                x = x / 3;
                if (first) {
                    runs = 1;
                    prev_digit = d;
                    first = 0;
                } else if (d != prev_digit) {
                    ++runs;
                    prev_digit = d;
                }
            }
        }

        /* if the run count is a multiple of 8, store the number */
        if (runs % 8 == 0) {
            if (length >= capacity) {
                capacity = capacity ? capacity << 1 : 1024;
                seq_t *tmp = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
                if (!tmp) return 0;          /* allocation failed */
                terms = tmp;
            }
            terms[length++] = k;
        }

        next = k;   /* next odd to test   */
    }

    /* return the nth term (1‑based indexing) */
    return terms[(size_t)n - 1];
}
