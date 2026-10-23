/*
 * A161649  Number of reduced words of length n in the Weyl group A_36.
 * https://oeis.org/A161649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Prod_{k=1..36} (1 + x + ... + x^k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    /* Allocate two buffers to alternate between stages */
    seq_t *cur  = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!cur || !next)
    {
        free(cur);
        free(next);
        return 0;   /* failure: memory allocation failed */
    }

    /* initialise current buffer to the unit polynomial 1 */
    for (size_t i = 0; i <= N; ++i) cur[i] = 0;
    cur[0] = 1;

    /* Iterate over factors k = 1 .. 36, each contributes a polynomial
       1 + x + ... + x^k (degree k) with all coefficients equal to 1 */
    for (idx_t k = 1; k <= 36; ++k)
    {
        size_t d = (size_t)(k + 1);           /* degree+1 */
        /* zero next buffer */
        for (size_t i = 0; i <= N; ++i) next[i] = 0;

        for (size_t j = 0; j <= N; ++j)
        {
            size_t max_i = j < d ? j : d - 1; /* maximum i such that j-i >= 0 and i < d */
            for (size_t i = 0; i <= max_i; ++i)
            {
                next[j] += cur[j - i];
            }
        }
        /* swap current and next buffers */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t result = cur[N];
    free(cur);
    free(next);
    return result;
}
