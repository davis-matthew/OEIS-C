/*
 * A060778  a(n) = gcd(tau(n+1), tau(n)), where tau = A000005.
 * https://oeis.org/A060778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(tau(n+1), tau(n))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 844 (2026-11-01)
 * Helpers:   gcd, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    uint64_t n64 = (uint64_t)n;
    seq_t tau_n   = (seq_t)num_divisors(n64);
    seq_t tau_np1 = (seq_t)num_divisors(n64 + 1);
    return gcd(tau_n, tau_np1);
}
