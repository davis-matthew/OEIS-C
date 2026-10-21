/*
 * A088878  Prime numbers p such that 3p - 2 is a prime.
 * https://oeis.org/A088878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 585, u32 n <= 586, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: scan primes p starting from 3 and count those for which 3p-2 is
   also prime. Return the n‑th such prime.  This matches OEIS A088878. */
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t p = 3;            /* first prime candidate */
    while (1) {
        /* compute 3p-2 as seq_t and test primality */
        seq_t val = (seq_t)3 * (seq_t)p - (seq_t)2;
        if (is_prime_big(val)) {
            ++count;
            if (count == n)
                return (seq_t)p;
        }
        /* advance to the next prime larger than current p */
        p = next_prime(p + 1);
    }
}
