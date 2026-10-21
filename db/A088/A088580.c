/*
 * A088580  a(n) = 1 + sigma(n).
 * https://oeis.org/A088580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + sigma(n)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_plus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where sigma(n) = sum of all positive divisors of n  */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sigma_plus_one.h"

seq_t a(idx_t n)
{
    return sigma_plus_one(n);
}
