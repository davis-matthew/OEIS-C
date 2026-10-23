/*
 * A013966  a(n) = sigma_18(n), the sum of the 18th powers of the divisors of
 *          n.
 * https://oeis.org/A013966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sigma_18(n) (sum of the 18th powers of the divisors of n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 11, u128 n <= 138,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    /* The OEIS definition starts at n = 1; for n = 0 return 0. */
    if (n == 0)
        return 0;
    return sigma_k(n, 18);
}
