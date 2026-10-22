/*
 * A000015  Smallest prime power >= n.
 * https://oeis.org/A000015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search upward from n for the first m that is 1 or a prime power
 *            (p^k, k >= 1)
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_power_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_power_ull.h"

seq_t a(idx_t n)
{
    unsigned long long m = (unsigned long long)n;
    if (m < 1) m = 1;
    while (!is_prime_power_ull(m)) m++;
    return (seq_t)m;
}
