/*
 * A184357  a(n) = Sum_{k=0..n} C(n^2-k^2, n-k)*C(k^2, k).
 * https://oeis.org/A184357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n^2 - k^2, n - k) * binomial(k^2,
 *            k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t n2 = n * n;                 /* n^2 fits in idx_t for the intended n range */

    for (idx_t k = 0; k <= n; ++k) {
        idx_t k2 = k * k;             /* k^2 */
        seq_t term1 = binomial(n2 - k2, n - k);   /* C(n^2‑k^2, n‑k) */
        seq_t term2 = binomial(k2, k);            /* C(k^2, k) */
        result += term1 * term2;     /* accumulation in seq_t */
    }
    return result;
}
