/*
 * A171783  Third smallest divisor of smallest number having exactly n
 *          divisors.
 * https://oeis.org/A171783
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 if n is prime, otherwise 3 (for n ≥ 3)
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;              /* undefined below the offset */

    return is_prime(n) ? (seq_t)4 : (seq_t)3;
}
