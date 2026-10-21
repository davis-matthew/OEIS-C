/*
 * A077134  Duplicate of A001710.
 * https://oeis.org/A077134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0) = 1, a(1) = 1, a(n) = n! / 2 for n ≥ 2 (since n! is even
 *            for n≥2, this is an integer)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 12, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache of already computed values. */
    static seq_t *tab = NULL;
    static idx_t cap = 0;
    static idx_t sz  = 0;   /* number of elements stored (last index + 1) */

    /* Initialise cache on first call. */
    if (tab == NULL) {
        cap = 4;                     /* start with capacity 4 */
        tab = (seq_t*)malloc(cap * sizeof(seq_t));
        tab[0] = 1;                   /* a(0) = 1 */
        tab[1] = 1;                   /* a(1) = 1 */
        tab[2] = 1;                   /* a(2) = 2!/2 = 1 */
        sz  = 3;                      /* elements 0,1,2 are defined */
    }

    /* Grow the table if needed. */
    while (n >= cap) {
        idx_t newcap = cap * 2;
        if (newcap == 0) newcap = 4;
        tab = (seq_t*)realloc(tab, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = sz; i <= n; ++i) {
        if (i == 2) {
            tab[i] = 1;                  /* already known, but keep for completeness */
        } else if (i >= 3) {
            tab[i] = (seq_t)i * tab[i - 1];
        } else {
            /* i is 0 or 1; already set. */
        }
    }
    sz = n + 1;

    return tab[n];
}
