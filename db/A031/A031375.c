/*
 * A031375  a(n) = prime(9n-1).
 * https://oeis.org/A031375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(9n-1)
 * Range:     u8 n <= 6, u16 n <= 727, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1-based; compute index of prime: 9n-1 */
    idx_t k = 9 * n - 1;
    return (seq_t) get_prime(k);
}
