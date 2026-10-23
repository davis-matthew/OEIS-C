/*
 * A020549  a(n) = (n!)^2 + 1.
 * https://oeis.org/A020549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2 + 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of factorial values: fac[i] = i!  (i >= 0) */
    static seq_t *fac = NULL;
    static idx_t fac_max = 0;  /* highest i for which fac[i] is valid */

    /* Initialise cache for i = 0 */
    if (fac == NULL) {
        fac = (seq_t *)malloc(sizeof(seq_t));
        if (fac == NULL) abort();
        fac[0] = (seq_t)1;
        fac_max = 0;
    }

    /* Extend cache to at least n */
    if (n > fac_max) {
        /* Allocate space for indices 0 .. n */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(fac, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        fac = tmp;
        /* Compute missing factorials */
        for (idx_t i = fac_max + 1; i <= n; ++i)
            fac[i] = fac[i - 1] * (seq_t)i;
        fac_max = n;
    }

    seq_t f = fac[n];
    return f * f + (seq_t)1;
}
