/*
 * A064652  Primes q associated with A064632.
 * https://oeis.org/A064652
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest prime q such that (q−1)*n + 1 is prime
 * Range:     u8 n <= 30, u16 n <= 4450, u32 n <= 4371, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is assumed ≥ 2. Return the smallest prime q for which
       (q-1)*n + 1 is also prime. */
    seq_t q = 2;          /* start with the smallest prime */
    while (1) {
        /* Test whether q itself is prime */
        if (is_prime_big(q)) {
            /* Compute p = (q-1)*n + 1 using seq_t arithmetic */
            seq_t p = ((q - (seq_t)1) * ((seq_t)n)) + (seq_t)1;
            if (is_prime_big(p)) {
                return q;
            }
        }
        /* Advance to the next candidate prime */
        if (q == (seq_t)2) {
            q = (seq_t)3;
        } else {
            q += (seq_t)2;
        }
    }
}
