/*
 * A175330  a(n) = bitwise AND of prime(n) and prime(n+1).
 * https://oeis.org/A175330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = prime(n) & prime(n+1)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n >= 1, indices are 1‑based */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);
    return (seq_t)((uint32_t)p1 & (uint32_t)p2);
}
