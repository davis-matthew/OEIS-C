/*
 * A085982  Number of 8's in decimal expansion of prime(n).
 * https://oeis.org/A085982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   number of decimal digit 8 in prime(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1u) return 0;
    uint32_t p = get_prime(n);          /* p is the n-th prime (1-indexed) */
    seq_t count = 0;
    while (p > 0) {
        if ((p % 10u) == 8u) ++count;
        p /= 10u;
    }
    return count;
}
