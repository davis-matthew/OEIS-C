/*
 * A121675  a(n) = [x^n] (1 + x*(1+x)^(n+1) )^n.
 * https://oeis.org/A121675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * C((n+1)*k, n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t m = n + 1;                     /* multiplier (n+1)          */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t c1 = binomial(n, k);           /* first binomial C(n,k)     */
        seq_t c2 = binomial(m * k, n - k);   /* second binomial           */
        sum += c1 * c2;                  /* contribution to a(n)      */
    }
    return sum;
}
