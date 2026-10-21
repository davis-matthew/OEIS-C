/*
 * A055209  a(n) = Product_{i=0..n} i!^2.
 * https://oeis.org/A055209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Prod_{i=0..n} (i!)^2 (a(0)=1, a(n) = a(n-1)*(n!)^2)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *a_vals  = NULL; /* array of a(k) values */
    static size_t  a_cap   = 0;    /* allocated capacity of a_vals */
    static idx_t   a_max   = 0;    /* largest index cached */

    static seq_t fact = 1;          /* current factorial value: fact = (fact_n)! */
    static idx_t  fact_n = 0;       /* last n for which fact == n!  */

    /* Initialise cache at first use */
    if (a_vals == NULL) {
        a_cap = 1;
        a_vals = (seq_t *)malloc(sizeof(seq_t) * a_cap);
        if (a_vals == NULL) return 0; /* memory failure */
        a_vals[0] = 1;                /* a(0) = 1 */
        a_max = 0;
        fact = 1;
        fact_n = 0;
    }

    /* if the requested value is already cached, return it */
    if (n <= a_max) {
        return a_vals[(size_t)n];
    }

    /* Grow the cache array if necessary */
    if ((size_t)(n + 1) > a_cap) {
        size_t new_cap = a_cap ? a_cap * 2 : 1;
        if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* memory failure */
        a_vals = tmp;
        a_cap = new_cap;
    }

    /* Compute new entries from a_max+1 up to n */
    for (idx_t i = a_max + 1; i <= n; ++i) {
        /* Update factorial: fact = i! */
        fact *= (seq_t)(i);
        fact_n = i;

        /* a(i) = a(i-1) * (i!)^2 */
        seq_t fact_sq = fact * fact;
        a_vals[(size_t)i] = a_vals[(size_t)(i - 1)] * fact_sq;
    }

    a_max = n;
    return a_vals[(size_t)n];
}
