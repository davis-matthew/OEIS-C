/*
 * A177992  Triangle read by rows, A007318 * A177990.
 * https://oeis.org/A177992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattening of the triangle T(r,k) defined by T(r,k)
 *            = binomial(r,k) if k is even = Σ_{j=0}^{r−k} 2^j *
 *            binomial(r−j−1, k−1) if k is odd where r is the row such that
 *            r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and k = n − r*(r+1)/2
 * Range:     u8 n <= 39, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;                 /* column within the row */

    if ((k & 1) == 0) {                   /* even column: binomial */
        return binomial(r, k);
    } else {                               /* odd column: weighted sum */
        seq_t sum = 0;
        seq_t pow = 1;                     /* 2^0 */
        for (idx_t j = 0; j <= r - k; ++j) {
            seq_t bin = binomial(r - j - 1, k - 1);
            sum += bin * pow;
            pow *= (seq_t)2;               /* prepare 2^(j+1) for next iteration */
        }
        return sum;
    }
}
