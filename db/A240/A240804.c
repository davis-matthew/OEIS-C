/*
 * A240804  a(n) = -2 + product of first n odd primes.
 * https://oeis.org/A240804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* formula: a(n) = -2 + product of first n odd primes */
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset starts at 1; this case is unused   */
    seq_t prod = 1;
    /* 2 is the first prime, so the k-th odd prime is get_prime(k+1)       */
    for (idx_t i = 1; i <= n; ++i) {
        uint32_t p = get_prime(i + 1);
        prod = prod * (seq_t)p;
    }
    prod = prod - (seq_t)2;
    return prod;
}
