/*
 * A038216  Triangle whose (i,j)-th entry is binomial(i,j)*2^(i-j)*10^j.
 * https://oeis.org/A038216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   a(n) = binomial(r,k) * 2^(r-k) * 10^k where r =
 *            floor((sqrt(8n+1)-1)/2) (row index), k = n - r*(r+1)/2 (column
 *            index within the row)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binomial, pow10, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((idx_t)(r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Column index within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute the binomial coefficient and multiply by powers of 2 and 10 */
    seq_t value = binomial(r, k);
    value *= pow2(r - k);
    value *= pow10(k);

    return value;
}
