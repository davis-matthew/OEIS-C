/*
 * A085393  Difference between the largest and the smallest prime factor of
 *          the greatest proper divisor of n.
 * https://oeis.org/A085393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest_prime_factor(n / smallest_prime_factor(n)) -
 *            smallest_prime_factor(n / smallest_prime_factor(n))
 * Range:     u8 n <= 1051, u16 n <= 9109, u32 n <= 9109, u64 n <= 9109,
 *            u128 n <= 9109, big any n
 * Helpers:   _smallest_prime_factor, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    uint64_t un = (uint64_t)n;
    uint64_t spf = _smallest_prime_factor(un);
    if (spf == 0 || spf == un) return 0;          // prime or 1

    uint64_t d = un / spf;                        // greatest proper divisor
    if (d <= 1) return 0;

    uint64_t spf_d = _smallest_prime_factor(d);
    if (spf_d == 0) return 0;                     // just in case

    uint64_t lpf_d = largest_prime_divisor(d);     // largest prime factor of d

    return (seq_t)(lpf_d - spf_d);
}
