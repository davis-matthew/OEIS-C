/*
 * A089086  Greatest common divisor of n^2-5 and n^2+5.
 * https://oeis.org/A089086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = gcd(n+5, 10)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute (n+5) mod 10 and take gcd with 10 */
    uint64_t r = ((uint64_t)n % 10ULL + 5ULL) % 10ULL;
    uint64_t g;
    if (r % 10ULL == 0ULL)
        g = 10ULL;
    else if (r % 5ULL == 0ULL)
        g = 5ULL;
    else if (r % 2ULL == 0ULL)
        g = 2ULL;
    else
        g = 1ULL;
    return (seq_t)g;
}
