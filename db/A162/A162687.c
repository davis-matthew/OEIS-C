/*
 * A162687  Write the distinct primes dividing n in binary, in order with the
 *          largest prime on the left and smallest on the right. Concatenate,
 *          and convert to decimal to get a(n).
 * https://oeis.org/A162687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = decimal of the binary concatenation of the distinct prime
 *            divisors of n, written from largest to smallest
 * Range:     u8 n <= 101, u16 n <= 9689, u32 n <= 9652, u64 n <= 9652,
 *            u128 n <= 9652, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 (no prime divisors). */
    if (n <= 1) {
        return (seq_t)0;
    }

    /* Factor n into distinct primes.  factor_u64 returns the primes in
       ascending order. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;

    /* Process primes from largest to smallest. */
    for (size_t i = cnt; i-- > 0; ) {
        uint64_t p = primes[i];

        /* Compute the number of bits in the binary representation of p. */
        unsigned len = 0;
        uint64_t t = p;
        while (t) {
            t >>= 1;
            ++len;
        }

        /* Append the bits of p to the result.  Left‑shift by len and OR
           with the prime value. */
        result = (result << len) | (seq_t)p;
    }

    return result;
}
