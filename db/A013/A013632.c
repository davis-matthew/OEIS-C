/*
 * A013632  Difference between n and the next prime greater than n.
 * https://oeis.org/A013632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Prime(PrimePi(n)+1) - n (i.e., the smallest positive k
 *            such that n+k is prime)
 * Range:     u8 n <= 657, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t candidate = (seq_t)n + 1;   /* start with n+1 */

    /* Find the first prime > n */
    while (!is_prime_big(candidate)) {
        candidate += 1;
    }

    /* The difference to the prime is the sequence value */
    return candidate - (seq_t)n;
}
