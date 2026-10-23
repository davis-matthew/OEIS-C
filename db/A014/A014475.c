/*
 * A014475  Triangular array formed from odd elements to right of middle of
 *          rows of Pascal's triangle.
 * https://oeis.org/A014475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the n-th odd binomial coefficient C(r,k) with r ≥ 1 and k
 *            > r/2, enumerated by increasing r, then increasing k
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val   = 0;

    for (idx_t r = 1; ; ++r) {
        idx_t start = r / 2 + 1;                     /* first index strictly right of middle */
        for (idx_t k = start; k <= r; ++k) {
            if ((k & (r - k)) == 0) {                /* C(r,k) is odd iff k & (r-k) == 0 */
                ++count;
                if (count == n) {
                    val = binomial(r, k);            /* exact binomial coefficient */
                    return val;
                }
            }
        }
    }
}
