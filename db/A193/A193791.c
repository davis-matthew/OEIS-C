/*
 * A193791  Mirror of the triangle A193790.
 * https://oeis.org/A193791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3^(r-1) if r-k = r (first element of row r) * = 1 if r-k
 *            = 0 (last element of row r) * = 2^(r-k) * binomial(r-1, r-k)
 *            otherwise * * Here r is the row number (starting at 0) such that
 *            triangular(r) <= n < triangular(r+1) * and k = n - triangular(r)
 *            is the position within the row
 * Range:     u8 n <= 27, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find row r where triangular(r) <= n < triangular(r+1) */
    idx_t r = (idx_t)((sqrt((long double)n * 8.0L + 1.0L) - 1.0L) / 2.0L);
    /* Adjust to be exact due to possible floating point rounding */
    idx_t t = r * (r + 1) / 2;
    while (t > n) {
        --r;
        t = r * (r + 1) / 2;
    }
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
        t = r * (r + 1) / 2;
    }
    idx_t k = n - t;          /* position within the row (0 <= k <= r) */
    idx_t kp = r - k;         /* index in the original (mirror) triangle */

    if (r == 0)                /* only element is 1 */
        return (seq_t)1;

    if (kp == r)               /* first element of the row: 3^(r-1) */
        return pow3(r - 1);

    if (kp == 0)               /* last element of the row: 1 */
        return (seq_t)1;

    /* interior element: 2^kp * binomial(r-1, kp) */
    seq_t part1 = pow2(kp);
    seq_t part2 = binomial(r - 1, kp);
    return part1 * part2;
}
