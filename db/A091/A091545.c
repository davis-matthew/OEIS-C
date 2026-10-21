/*
 * A091545  First column sequence of the array (7,2)-Stirling2 A091747.
 * https://oeis.org/A091545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{j=0}^{n-1} ((5*j+2)*(5*j+1)) / 2 (offset 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *values = NULL;
    static idx_t computed = 0;          /* number of values already stored (>=1) */

    if (n == 0) {                        /* undefined for n=0, return 0 */
        return (seq_t)0;
    }

    if (n <= computed) {
        return values[n - 1];
    }

    /* grow storage */
    size_t new_size = (size_t)n;
    seq_t *new_vals = (seq_t *)realloc(values, new_size * sizeof(seq_t));
    if (!new_vals) {                     /* allocation failed – abort */
        return (seq_t)0;
    }
    values = new_vals;

    /* initialise first entry if necessary */
    if (computed == 0) {
        values[0] = (seq_t)1;           /* a(1) = 1 */
        computed = 1;
    }

    /* compute missing values up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t tk = (seq_t)k;
        seq_t factor1 = (seq_t)5 * tk - (seq_t)3;   /* 5*k - 3 */
        seq_t factor2 = (seq_t)5 * tk - (seq_t)4;   /* 5*k - 4 */
        values[k - 1] = factor1 * factor2 * values[k - 2];
    }

    computed = n;
    return values[n - 1];
}
