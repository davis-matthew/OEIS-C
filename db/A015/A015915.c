/*
 * A015915  Numbers k such that sigma(k) + 8 = sigma(k+8).
 * https://oeis.org/A015915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 896, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A015915: numbers k such that sigma(k) + 8 = sigma(k+8). */
#include <stdlib.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Static state for memoised search */
    static idx_t      current = 1;      /* current candidate to test   */
    static idx_t      found   = 0;      /* number of terms found so far */
    static seq_t      *list   = NULL;   /* array of found terms         */
    static size_t     cap     = 0;      /* capacity of list             */

    /* Initialise sigma_next8 only once */
    static seq_t sigma_next8 = 0;
    if (list == NULL) {
        sigma_next8 = sigma_of(current + 8);
    }

    /* Generate terms until we have at least n of them */
    while (found < n) {
        seq_t s = sigma_of(current) + (seq_t)8;
        if (s == sigma_next8) {
            /* Store found term */
            if (found >= cap) {
                cap = cap ? cap * 2 : 64;
                list = (seq_t *)realloc(list, cap * sizeof(seq_t));
            }
            list[found++] = (seq_t)current;
        }
        /* Advance to the next candidate */
        ++current;
        sigma_next8 = sigma_of(current + 8);
    }

    return list[n - 1];
}
