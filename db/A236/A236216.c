/*
 * A236216  Sum of the tenth powers of the first n primes.
 * https://oeis.org/A236216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} prime(k)^10
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 21, u128 n <= 646,
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
    /* If called with n == 0 (outside the official offset) return 0. */
    if (n == 0)
        return 0;

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        uint32_t prime_val = get_prime(k);
        seq_t p10 = big_pow((seq_t)prime_val, (seq_t)10);
        sum += p10;
    }

    return sum;
}
