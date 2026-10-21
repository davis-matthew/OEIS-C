/*
 * A088683  a(n) = prime(3*n+2) - prime(3*n).
 * https://oeis.org/A088683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(3*n+2) - prime(3*n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence is defined for n ≥ 1 */

    /* Compute indices of the two primes */
    idx_t k = 3 * n;               /* 3*n */
    uint32_t p1 = get_prime(k);    /* prime(3*n)   */
    uint32_t p2 = get_prime(k + 2);/* prime(3*n+2) */

    return (seq_t)(p2 - p1);
}
