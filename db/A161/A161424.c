/*
 * A161424  Numbers k whose largest divisor <= sqrt(k) equals 4.
 * https://oeis.org/A161424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*(n+3) for n<=5; a(n) = 4*prime(n-1) for n>5
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n <= 5)
        return ((seq_t)4) * ((seq_t)n + 3);

    idx_t prime_index = n - 1;
    uint32_t p = get_prime(prime_index);
    return ((seq_t)4) * ((seq_t)p);
}
