/*
 * A131233  a(n) is the number of positive integers <= n that do not have 2 or
 *          more distinct prime divisors in common with n.
 * https://oeis.org/A131233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers m ≤ n such that gcd(m,n) is 1 or a
 *            prime power
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime_power_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_power_u64.h"

/* Test whether x is a prime power (x ≥ 0).  Returns true for 0,1 or any prime power. */

seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t m = 1; m <= n; ++m) {
        seq_t g = gcd((seq_t)m, (seq_t)n);
        if (is_prime_power_u64((uint64_t)g))
            count++;
    }

    return count;
}
