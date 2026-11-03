/*
 * A229338  Product of products of elements of subsets of divisors of n.
 * https://oeis.org/A229338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(tau(n)*2^(tau(n)-2)) where tau(n) = A000005(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-01)
 * Helpers:   ipow, num_divisors, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow.h"
#include "../utils/num_divisors.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* special case: a(1) = 1 */

    idx_t tau = num_divisors(n);   /* number of divisors of n */

    /* exponent e = tau * 2^(tau-2) */
    seq_t e = pow2(tau - 2) * (seq_t)tau;

    seq_t base = (seq_t)n;
    return ipow(base, e);
}
