/*
 * A013972  a(n) = sigma_24(n), the sum of the 24th powers of the divisors of
 *          n.
 * https://oeis.org/A013972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sigma_24(n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 40,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    /* Sum of the 24th powers of the divisors of n. */
    return sigma_k(n, 24);
}
