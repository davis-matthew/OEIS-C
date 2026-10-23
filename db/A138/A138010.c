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
 * Helpers:   divisor_count_u64, u64_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/u64_gcd.h"

seq_t a(idx_t n)
{
    /* n fits in uint64_t by construction (idx_t is uint64_t) */
    uint64_t nn = (uint64_t)n;

    /* d(n) – number of positive divisors of n */
    uint64_t d = divisor_count_u64(nn);

    /* g = gcd(n, d(n)) */
    uint64_t g = u64_gcd(nn, d);

    /* a(n) = d(g) */
    uint64_t ret = divisor_count_u64(g);

    return (seq_t)ret;
}
