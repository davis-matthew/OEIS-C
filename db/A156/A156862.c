/*
 * A156862  Triangle read by rows: T(n, k) = 2^k - binomial(n, k+1) + 2^(n-k)
 *          - binomial(n, n-k+1).
 * https://oeis.org/A156862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8n+1)−1)/2⌋, k = n−r(r+1)/2, and
 *            T(n,k) = 2^k − C(n,k+1) + 2^(n−k) − C(n,n−k+1)
 * Range:     i8 n <= 35, i16 n <= 135, i32 n <= 527, i64 n <= 2079,
 *            i128 n <= 5150, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* locate the row r : r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        r++;

    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    seq_t val = 0;
    val += pow2(k);
    val -= binomial(r, k + 1);
    val += pow2(r - k);
    val -= binomial(r, r - k + 1);

    return val;
}
