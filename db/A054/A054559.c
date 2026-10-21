/*
 * A054559  Number of labeled pure 2-complexes on n nodes (0-simplexes) with 5
 *          2-simplexes and 8 1-simplexes.
 * https://oeis.org/A054559
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n-3)*(n-4)/4 for n >= 5 (This equals
 *            30*C(n,5) as given in the OEIS entry.)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 113, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 5.  For smaller n we return
       zero; the caller is not expected to call a(n) with n < 5. */
    if (n < 5)
        return (seq_t)0;

    seq_t result = (seq_t)1;
    bool div_done = false;

    /* Compute the product of n, n-1, n-2, n-3, n-4, but divide by 4
       as early as possible to keep the intermediate size small. */
    for (idx_t k = 0; k < 5; ++k)
    {
        seq_t term = (seq_t)(n - k);

        /* Divide by 4 once, if possible, before multiplication. */
        if (!div_done && (term % (seq_t)4) == 0)
        {
            term /= (seq_t)4;
            div_done = true;
        }

        result *= term;
    }

    return result;
}
