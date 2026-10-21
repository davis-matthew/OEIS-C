/*
 * A055303  Number of labeled rooted trees with n nodes and 2 leaves.
 * https://oeis.org/A055303
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * (n-2) * (n-1) / 4 Recurrence: a(3) = 3, a(n) =
 *            (a(n-1) / (n-3)) * n * (n-1) for n ≥ 4 The division is exact
 *            because (n-3) divides a(n-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Undefined below offset; return 0. */
    if (n < 3)
        return (seq_t)0;

    /* Static memoisation storage. */
    static seq_t *tbl = NULL; /* array of values a(i) */
    static idx_t sz  = 0;     /* current allocated capacity */
    static idx_t last = 0;    /* largest index computed */

    /* Ensure table large enough for index n. */
    if (n + 1 > sz) {
        idx_t new_sz = n + 1;
        tbl = (seq_t *)realloc(tbl, new_sz * sizeof(seq_t));
        sz = new_sz;
    }

    /* Initialise first defined term if needed. */
    if (last < 3) {
        tbl[3] = (seq_t)3;
        last   = 3;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* a(i) = (a(i-1) / (i-3)) * i * (i-1)   */
        seq_t tmp = tbl[i - 1] / (i - 3); /* exact division */
        tmp *= (seq_t)i;
        tmp *= (seq_t)(i - 1);
        tbl[i] = tmp;
    }

    last = n;
    return tbl[n];
}
