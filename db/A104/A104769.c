/*
 * A104769  Expansion of g.f. -x/(1+x-x^3).
 * https://oeis.org/A104769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n+3) = a(n) - a(n+2); a(0) = 0, a(1) = -1, a(2) = 1
 * Range:     i8 n <= 36, i16 n <= 76, i32 n <= 156, i64 n <= 313,
 *            i128 n <= 629, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static memoization table */
    static seq_t *tbl = NULL;
    static idx_t sz = 0;   /* allocated size */
    static idx_t len = 0;  /* number of initialized elements */

    /* enlarge allocation if necessary */
    if (n >= sz) {
        idx_t new_sz = (n + 1) > 4 ? (n + 1) : 4;   /* start with at least 4 slots */
        tbl = (seq_t *)realloc(tbl, (size_t)new_sz * sizeof(seq_t));
        sz = new_sz;
    }

    /* initialise first three terms once */
    if (len == 0) {
        tbl[0] = (seq_t)0;
        tbl[1] = (seq_t)-1;
        tbl[2] = (seq_t)1;
        len = 3;
    }

    /* compute any missing terms up to n */
    if (n >= len) {
        for (idx_t i = len; i <= n; ++i) {
            tbl[i] = tbl[i - 3] - tbl[i - 1];
        }
        len = n + 1;
    }

    return tbl[n];
}
