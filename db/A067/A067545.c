/*
 * A067545  Determinant of an n X n matrix whose diagonal are the first n
 *          composite numbers and all other elements are 1's.
 * https://oeis.org/A067545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1}^n (c_i-1) + Σ_{i=1}^n ∏_{j≠i} (c_j-1), where c_i
 *            is the i‑th composite number
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term a(n) of OEIS sequence A067545. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset starts at 1, but protect against 0 */

    /* allocate array for d_i = composite-1 */
    seq_t *d = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (d == NULL)
        return 0;          /* allocation failed, unlikely for these sizes */

    /* gather the first n composite numbers, storing d_i = c_i - 1 */
    idx_t count = (idx_t)0;
    seq_t m = (seq_t)4;   /* first composite number */
    while (count < n)
    {
        if (!is_prime_small(m))
        {
            d[count] = m - (seq_t)1;
            ++count;
        }
        ++m;
    }

    /* compute product P = ∏ d_i */
    seq_t prod = (seq_t)1;
    for (idx_t i = (idx_t)0; i < n; ++i)
        prod = prod * d[i];

    /* compute a(n) = P + Σ P / d_i */
    seq_t sum = prod;
    for (idx_t i = (idx_t)0; i < n; ++i)
        sum = sum + (prod / d[i]);

    free(d);
    return sum;
}
