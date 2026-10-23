/*
 * A171784  Fourth smallest divisor of smallest number having exactly n
 *          divisors.
 * https://oeis.org/A171784
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8 if n is prime; a(4) = 6; otherwise a(n) = 4
 * Range:     u8 n <= 9615, u16 n <= 9615, u32 n <= 9615, u64 n <= 9615,
 *            u128 n <= 9615, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)4)
        return (seq_t)6;
    if (is_prime(n))
        return (seq_t)8;
    return (seq_t)4;
}
