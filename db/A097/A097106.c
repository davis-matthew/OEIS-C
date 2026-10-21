/*
 * A097106  a(n) = (Smallest prime power >= n) - (greatest prime power <= n).
 * https://oeis.org/A097106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (smallest prime power >= n) - (greatest prime power <= n)
 * Range:     u8 n <= 9316, u16 n <= 9316, u32 n <= 9316, u64 n <= 9316,
 *            u128 n <= 9316, big any n
 * Helpers:   is_prime_power_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0; a(n) = 0 iff n itself is a (ordinary) prime power. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_power_uint64.h"

/* Main entry point for the OEIS sequence.  */
seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* a(1) is defined as 0 */

    idx_t low = n;
    while (low >= 2 && !is_prime_power_uint64((uint64_t)low))
        --low;                     /* lower bound, at least 2 */

    idx_t high = n;
    while (!is_prime_power_uint64((uint64_t)high))
        ++high;                    /* upper bound */

    return (seq_t)(high - low);
}
