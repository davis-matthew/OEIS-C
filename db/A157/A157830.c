/*
 * A157830  Let p(x) = 1 + 759*x^8 + 2576*x^12 + 759*x^16 + x^24, expansion of
 *          the reciprocal polynomial of p(x).
 * https://oeis.org/A157830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, less
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 27, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; for n ≥ 1
   a(n) = –(759·a(n–8) + 2576·a(n–12) + 759·a(n–16) + a(n–24)),
   with a(k) = 0 for k < 0. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;  /* pointer to array of seq_t   */
    static size_t cap   = 0;     /* allocated capacity in elems */
    static size_t comp  = 0;     /* number of values already computed  */

    /* enlarge cache if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1) * 2;          /* overallocate a bit */
        if (cache == NULL)
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute values from first uncomputed index up to n */
    for (size_t i = comp; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = 1;          /* base case */
            continue;
        }

        seq_t sum = 0;
        if (i >= 8)  sum += (seq_t)759  * cache[i - 8];
        if (i >= 12) sum += (seq_t)2576 * cache[i - 12];
        if (i >= 16) sum += (seq_t)759  * cache[i - 16];
        if (i >= 24) sum +=           cache[i - 24];

        cache[i] = -sum;
    }
    comp = (size_t)n + 1;  /* all values up to n are now cached */

    return cache[n];
}
