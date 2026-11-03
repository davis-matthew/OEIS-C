/*
 * A063026  Reversion of y - y^2 + y^4 - y^5.
 * https://oeis.org/A063026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A063026: Reversion of y - y² + y⁴ - y⁵
   Formula: a(0) = 0
   a(n+1) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} (-1)^k *
              binomial(n+k, n) * binomial(2*n-3*k, n)
   (n ≥ 0)
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 */

    /* m = n-1  →  a(n) = 1/n * Σ_{k=0..⌊(n-1)/3⌋} (-1)^k *
       binomial((n-1)+k, n-1) * binomial(2*(n-1)-3*k, n-1) */
    idx_t m = n - 1;
    idx_t maxk = m / 3;
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial(m + k, m);          /* binomial((n-1)+k, n-1) */
        seq_t t2 = binomial(2 * m - 3 * k, m);  /* binomial(2*(n-1)-3k, n-1) */
        seq_t term = t1 * t2;
        if (k & 1)
            sum -= term;   /* (-1)^k with k odd */
        else
            sum += term;   /* (-1)^k with k even */
    }

    return sum / (seq_t)n;   /* division is exact by the formula */
}
