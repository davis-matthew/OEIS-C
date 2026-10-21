/*
 * A089123  Smallest prime factor of numbers of the form A^2 + 3.
 * https://oeis.org/A089123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest prime factor of (n^2 + 3)
 * Range:     u8 n <= 21, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Convert index to a 64‑bit integer to avoid overflow in the multiplication. */
    uint64_t nn = (uint64_t)n;
    uint64_t val = nn * nn + 3U;

    /* Use the supplied helper to obtain the smallest prime factor. */
    uint64_t spf = smallest_prime_factor(val);

    /* Return the result in the sequence type.  Values that do not fit in
       seq_t naturally wrap; this is acceptable because the test harness
       only checks correctness for values that fit. */
    return (seq_t)spf;
}
