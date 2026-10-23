/*
 * A014476  Triangular array formed from even elements to right of middle of
 *          rows of Pascal's triangle.
 * https://oeis.org/A014476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = nth even binomial coefficient C(r,k) with k > r/2,
 *            scanning rows r = 1,2,3,... of Pascal's triangle in order
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; guard against zero */
        return 0;

    idx_t count = 0;               /* number of qualifying coefficients seen */
    for (idx_t r = 1; ; ++r)       /* iterate over rows of Pascal's triangle */
    {
        idx_t mid = r / 2;           /* floor(r/2) */
        for (idx_t k = mid + 1; k <= r; ++k)  /* entries strictly to the right of the middle */
        {
            seq_t val = binomial(r, k);   /* C(r,k) */
            if ((val & (seq_t)1) == 0)    /* even? */
            {
                ++count;
                if (count == n)
                    return val;          /* nth term found */
            }
        }
    }
}
