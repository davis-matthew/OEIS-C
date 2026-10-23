/*
 * A166502  The n-th power of the product prime(n)*prime(n+1) of 2 successive
 *          primes.
 * https://oeis.org/A166502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (prime(n)*prime(n+1))^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);
    seq_t base = (seq_t)p1 * (seq_t)p2;
    return big_pow(base, (seq_t)n);
}
