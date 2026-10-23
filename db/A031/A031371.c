/*
 * A031371  a(n) = prime(5n-1).
 * https://oeis.org/A031371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(5n-1)
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    idx_t k = 5 * n - 1;      /* index of the desired prime */
    uint32_t p = get_prime(k);
    return (seq_t)p;
}
