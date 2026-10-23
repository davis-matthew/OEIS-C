/*
 * A142632  Primes congruent to 43 mod 55.
 * https://oeis.org/A142632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p % 55 == 43
 * Range:     u8 n <= 1, u16 n <= 135, u32 n <= 151, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1)   /* undefined for zero or negative indices */
        return (seq_t)0;

    seq_t cand = (seq_t)43;   /* first prime in the progression 43, 98, ...  */
    idx_t count = 1;          /* number of found primes so far */

    if (n == 1)
        return cand;

    while (count < n) {
        cand += (seq_t)55;    /* next candidate congruent to 43 mod 55   */
        if (is_prime_big(cand))
            ++count;
    }
    return cand;
}
