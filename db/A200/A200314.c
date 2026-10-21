/*
 * A200314  E.g.f. satisfies: A(x) = exp(x^4*A(x)^4/4!).
 * https://oeis.org/A200314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n+1)^(n-1) * (4*n)!/(n!*24^n) (offset 0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"   /* for pow_uint(uint64_t base, idx_t exp) */

seq_t a(idx_t n) {
    /* Handle the offset.  a(0) = 1 by definition. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute the first factor: (4*n+1)^(n-1) */
    seq_t first = pow_uint((uint64_t)(4 * n + 1), n - 1);

    /* Compute the product (n+1)(n+2)...(4*n) */
    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k <= 3 * n; ++k) {
        prod *= (seq_t)(n + k);
    }

    /* Divide by 24^n = (4!)^n */
    seq_t denom = pow_uint(24, n);
    seq_t part = prod / denom;

    /* Final result */
    return first * part;
}
