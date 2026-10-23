/*
 * A105853  a(n) = sigma(n) (mod 10), i.e., unit's digit of sigma(n).
 * https://oeis.org/A105853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = sigma(n) mod 10 where sigma(n) is the sum of all positive
 *            divisors of n
 * Range:     u8 n <= 9215, u16 n <= 9215, u32 n <= 9215, u64 n <= 9215,
 *            u128 n <= 9215, big any n
 * Helpers:   sigma_mod10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_mod10.h"

/* External interface required by the framework. */
seq_t a(idx_t n)
{
    return sigma_mod10(n);
}
