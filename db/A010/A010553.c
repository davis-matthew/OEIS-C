/*
 * A010553  a(n) = tau(tau(n)).
 * https://oeis.org/A010553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(tau(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9706 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t t1 = num_divisors((uint64_t)n);
    seq_t t2 = num_divisors((uint64_t)t1);
    return t2;
}
