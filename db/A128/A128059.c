/*
 * A128059  a(n) = numerator((2*n-1)^2/(2*(2*n)!)).
 * https://oeis.org/A128059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n-1) if (2*n-1) is prime, otherwise 1; a(0) = 1
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t m = (seq_t)n *   /* multiply by 2 using seq_t arithmetic */
              2;
    m -= 1;
    return is_prime_big(m) ? m : (seq_t)1;
}
