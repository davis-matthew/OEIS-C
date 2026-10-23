/*
 * A156189  Denominator of Euler(n, 1/6).
 * https://oeis.org/A156189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 36^(floor(n/2)) * (n odd ? 3 : 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 36^k where k = floor(n/2) using binary exponentiation. */
    uint64_t k = n >> 1;          /* floor(n/2) */
    seq_t result = 1;
    seq_t base   = 36;

    while (k > 0) {
        if (k & 1)
            result *= base;
        base *= base;
        k >>= 1;
    }

    /* If n is odd, multiply by an extra factor of 3. */
    if (n & 1)
        result *= 3;

    return result;
}
