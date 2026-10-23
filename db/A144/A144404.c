/*
 * A144404  Triangle T(n,k) = 3*binomial(n, k)^2 - binomial(n, k) - 1, read by
 *          rows.
 * https://oeis.org/A144404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3*binomial(r,k)^2 - binomial(r,k) - 1, where r = max t
 *            with t*(t+1)/2 <= n, k = n - r*(r+1)/2
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 178, u64 n <= 644,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    idx_t cumulative = 0;
    while (cumulative + r + 1 <= n) {
        cumulative += r + 1;
        ++r;
    }

    /* Column k */
    idx_t k = n - cumulative;

    /* Binomial coefficient */
    seq_t c = binomial(r, k);

    /* Compute the triangle entry: 3*c^2 - c - 1 */
    seq_t result = c * c;
    result *= 3;
    result -= c;
    result -= 1;
    return result;
}
