/*
 * A157983  a(n)=3!*n!/(8!*19!).
 * https://oeis.org/A157983
 *
 * Offset:    24    (a(24) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3!*n!/(8!*19!) = (n!)/(6720*19!)
 * Range:     u8 none, u16 n <= 25, u32 n <= 28, u64 n <= 35, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Values are defined for n >= 24 only (offset = 24).  */
    if (n < 24)
        return (seq_t)0;           /* undefined behavior for indices < 24 */

    /* Convert to offset relative to the first defined term: a(24). */
    idx_t off = n - 24;

    /* Static cache of computed values: values[0] == a(24), values[1] == a(25), … */
    static seq_t *values = NULL;   /* pointer to array of seq_t */
    static size_t values_len = 0;  /* number of entries allocated (>= off+1 when required) */

    /* Initial construction of the cache with a(24). */
    if (values_len == 0) {
        values_len = 1;
        values = (seq_t *)malloc(values_len * sizeof(seq_t));
        if (!values) return (seq_t)0;  /* allocation failure – unlikely in this context */
        values[0] = (seq_t)759;        /* a(24) = 759 */
    }

    /* If we don't have enough entries yet, grow the cache and compute the missing values. */
    if (off >= values_len) {
        size_t new_len = (size_t)off + 1;
        seq_t *new_arr = (seq_t *)realloc(values, new_len * sizeof(seq_t));
        if (!new_arr) return (seq_t)0; /* allocation failure */
        values = new_arr;

        /* Compute the new entries using the recurrence a(n+1) = (n+1)*a(n). */
        for (idx_t i = (idx_t)values_len; i <= off; ++i) {
            seq_t prev = values[i - 1];          /* previous term a(24 + i - 1) */
            seq_t factor = (seq_t)(i + 24);      /* (24 + i) == n for current term */
            values[i] = prev * factor;
        }
        values_len = new_len;
    }

    return values[off];
}
