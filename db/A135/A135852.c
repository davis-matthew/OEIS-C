/*
 * A135852  A007318 * A103516 as a lower triangular matrix.
 * https://oeis.org/A135852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n)th element of the triangle T with T(r,c) =
 *            (c+1)*C(r,c) for c>0 T(r,0) = (r+2)*2^(r-1) where r =
 *            floor((sqrt(8*n+1)-1)/2) and c = n - r*(r+1)/2. The triangle is
 *            given in row‑major order, so a(n) = T(r,c)
 * Range:     u8 n <= 20, u16 n <= 104, u32 n <= 434, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

/* Return the n'th term of A135852 */
seq_t a(idx_t n)
{
    /* Special case n==0 */
    if (n == 0) return (seq_t)1;

    /* Compute row r via floor((sqrt(8*n+1)-1)/2) */
    double d = sqrt((double)(8ULL * n + 1));
    idx_t r = ((idx_t)d - 1) / 2;

    /* Correct possible rounding errors */
    while (r * (r + 1) / 2 > n) r--;
    while ((r + 1) * (r + 2) / 2 <= n) r++;

    /* Column index */
    idx_t c = n - r * (r + 1) / 2;

    if (c == 0) {
        /* r >= 1 because n > 0 */
        seq_t pow_val = pow2(r - 1);
        return (seq_t)(r + 2) * pow_val;
    } else {
        seq_t bin = binomial(r, c);
        return (seq_t)(c + 1) * bin;
    }
}
