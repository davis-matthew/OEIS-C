/*
 * A031394  a(n) = prime(7*n - 4).
 * https://oeis.org/A031394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(7*n-4)
 * Range:     u8 n <= 8, u16 n <= 935, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    idx_t k = 7 * n - 4;
    return get_prime(k);
}
