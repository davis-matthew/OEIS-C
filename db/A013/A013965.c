/*
 * A013965  a(n) = sigma_17(n), the sum of the 17th powers of the divisors of
 *          n.
 * https://oeis.org/A013965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma_k(n, 17) = sum_{d|n} d^17
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 13, u128 n <= 184,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    return sigma_k(n, 17);
}
