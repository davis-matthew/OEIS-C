/*
 * A127990  Largest prime factor of 2*n^3 - 2*n + 9.
 * https://oeis.org/A127990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor of 2*n^3 - 2*n + 9
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 1860, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    /* No meaningful value for n < 1; return 0 */
    if (n < 1) return 0;

    uint64_t x = (uint64_t)n;
    unsigned __int128 n128 = x;
    unsigned __int128 v128 = 2 * n128 * n128 * n128 - 2 * n128 + 9;
    uint64_t v = (uint64_t)v128;  /* value of 2*n^3 - 2*n + 9 */

    uint64_t lpf = largest_prime_divisor(v);
    return (seq_t)lpf;
}
