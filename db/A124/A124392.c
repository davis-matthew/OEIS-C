/*
 * A124392  A Fine number related number triangle.
 * https://oeis.org/A124392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,c) where r is the row index with r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2, c = n - r(r+1)/2, and T(r,c) = Σ_{j=0..r-c}
 *            C(r-j,c) · C(2j,r-c)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 170, u64 n <= 594,
 *            u128 n <= 2210, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r such that n lies in that row. */
    idx_t r = 0;
    idx_t start = 0;           /* starting index of current row   */
    while (start + (r + 1) <= n) {
        start += r + 1;
        r++;
    }

    /* Column within the row. */
    idx_t c = n - start;       /* 0 ≤ c ≤ r                       */

    /* Compute T(r,c) = Σ binomial(r-j,c) * binomial(2*j,r-c). */
    seq_t sum = 0;
    idx_t maxj = r - c;
    for (idx_t j = 0; j <= maxj; ++j) {
        seq_t t1 = binomial(r - j, c);
        seq_t t2 = binomial(2 * j, r - c);
        sum += t1 * t2;
    }
    return sum;
}
