/*
 * A138010  a(n) is the number of positive divisors of n that divide d(n),
 *          where d(n) is the number of positive divisors of n, A000005(n);
 *          a(n) also equals d(gcd(n, d(n))).
 * https://oeis.org/A138010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(A009191(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 136 terms through n = 9428 (2026-11-01)
 * Helpers:   gcd_u64, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    /* n fits in uint64_t by construction (idx_t is uint64_t) */
    uint64_t nn = (uint64_t)n;

    /* d(n) – number of positive divisors of n */
    uint64_t d = num_divisors(nn);

    /* g = gcd(n, d(n)) */
    uint64_t g = gcd_u64(nn, d);

    /* a(n) = d(g) */
    uint64_t ret = num_divisors(g);

    return (seq_t)ret;
}
