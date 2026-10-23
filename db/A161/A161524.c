/*
 * A161524  Number of reduced words of length n in the Weyl group A_24.
 * https://oeis.org/A161524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in \prod_{k=1}^{24}(1-x^{k+1})/(1-x),
 *            which equals the coefficient of x^n in
 *            \prod_{k=1}^{24}(1+x+...+x^k). The polynomial has degree
 *            \sum_{k=1}^{24}k = 300
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 60, u128 n <= 300,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_DEG 300

seq_t a(idx_t n)
{
    static seq_t poly[MAX_DEG + 1];
    static bool initialized = false;

    if (!initialized)
    {
        /* initialise to zero */
        for (idx_t i = 0; i <= MAX_DEG; ++i)
            poly[i] = (seq_t)0;
        poly[0] = (seq_t)1;          /* start with polynomial 1 */
        idx_t degree = 0;

        for (idx_t k = 1; k <= 24; ++k)
        {
            /* zero the positions that will be written to this round */
            for (idx_t i = degree + 1; i <= degree + k; ++i)
                poly[i] = (seq_t)0;

            /* multiply by (1 + x + ... + x^k) in place,
               iterating from highest degree downwards. */
            for (long long i = degree; i >= 0; --i)
            {
                seq_t val = poly[i];
                if (val)
                {
                    for (idx_t j = 1; j <= k; ++j)
                        poly[i + j] += val;
                }
            }
            degree += k;
        }
        initialized = true;
    }

    return (n <= MAX_DEG) ? poly[n] : (seq_t)0;
}
