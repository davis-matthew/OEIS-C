/*
 * A060077  Fifth column of triangle A060074.
 * https://oeis.org/A060077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-fold iterated sum of squares with last upper summation
 *            index 5 (i.e. the n-th diagonal of triangle A060058)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* a(n) = A060077(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* number of indices needed in the innermost layer */
    size_t size = (size_t)n + 5;          /* index range 0 .. n+4   */
    seq_t *cur = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *tmp = (seq_t *)malloc(size * sizeof(seq_t));
    if (!cur || !tmp) {
        /* allocation failed – return 0 (should not happen in usual tests) */
        free(cur);
        free(tmp);
        return (seq_t)0;
    }

    /* initial layer: f_1(i) = Σ_{k=0}^i k^2  */
    for (size_t i = 0; i < size; ++i) {
        seq_t xi = (seq_t)i;
        cur[i] = xi * xi;
    }

    /* iterate depth d = 2 .. n  (i.e. n-1 times) */
    for (idx_t step = 1; step <= (idx_t)n - 1; ++step) {
        size_t new_size = size - 1;         /* next array is one element shorter */

        /* compute sums Σ_{k=0}^{j+1} cur[k] progressively */
        seq_t sum = cur[0] + cur[1];        /* j = 0 term uses cur[0] and cur[1] */
        tmp[0] = (seq_t)0 * (seq_t)0 * sum; /* j = 0 contributes 0 */

        for (size_t j = 1; j < new_size; ++j) {
            sum += cur[j + 1];              /* extend sum to include cur[j+1] */
            seq_t jj = (seq_t)j;
            tmp[j] = jj * jj * sum;
        }

        /* swap buffers for next iteration */
        seq_t *save = cur;
        cur = tmp;
        tmp = save;
        size = new_size;
    }

    /* final result: sum of the first six values (j = 0 .. 5) */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < 6 && i < size; ++i)
        result += cur[i];

    free(cur);
    free(tmp);
    return result;
}
