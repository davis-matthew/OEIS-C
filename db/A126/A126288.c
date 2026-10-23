/*
 * A126288  a(1) = 2, a(n) = n * LargestPrimeFactor(n+1) /
 *          LargestPrimeFactor(n).
 * https://oeis.org/A126288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * LargestPrimeFactor(n+1) / LargestPrimeFactor(n)
 * Range:     u8 n <= 35, u16 n <= 485, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; offset is 1 */
    if (n == 1) return 2;          /* a(1) = 2 */

    uint64_t lpf_n   = largest_prime_factor((uint64_t)n);
    uint64_t lpf_np1 = largest_prime_factor((uint64_t)n + 1);

    return (seq_t)n * (seq_t)lpf_np1 / (seq_t)lpf_n;
}
