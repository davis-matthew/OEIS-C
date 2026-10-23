/*
 * A117897  Number of labeled trees on prime numbers of nodes through n-th
 *          prime.
 * https://oeis.org/A117897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} prime(k)^(prime(k)-2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
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
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        uint32_t p = get_prime(k);          /* k-th prime */
        seq_t term = big_pow((seq_t)p, (seq_t)(p - 2));
        sum += term;
    }
    return sum;
}
