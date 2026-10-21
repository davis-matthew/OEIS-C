/*
 * A240233  a(n) is the smallest prime number such that both a(n) + 6n and
 *          a(n) + 12n are prime numbers.
 * https://oeis.org/A240233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) returns the smallest prime p such that p, p + 6*n and p +
 *            12*n are all prime
 * Range:     u8 n <= 20, u16 n <= 4055, u32 n <= 4583, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* For n >= 1.  The offset is 1, so a(1) is the first element. */
    if (n == 0) return 0;   /* undefined for offset, but harmless */

    seq_t n6  = (seq_t)6 * n;
    seq_t n12 = n6 * 2;

    seq_t p = 3;               /* smallest odd prime candidate */
    for (;;) {
        /* Check the triple of primes */
        if (is_prime_big(p) &&
            is_prime_big(p + n6) &&
            is_prime_big(p + n12))
            return p;

        /* Advance to the next prime */
        p = (seq_t)next_prime((uint64_t)p);
    }
}
