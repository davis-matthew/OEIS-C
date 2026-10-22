/*
 * A007529  Prime triples: p; p+2 or p+4; p+6 all prime.
 * https://oeis.org/A007529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 368, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Search: primes p where p+6 is prime and at least one of p+2, p+4 is prime */

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t p = 5;
    
    while (true) {
        if (is_prime(p) && is_prime(p + 6) && (is_prime(p + 2) || is_prime(p + 4))) {
            count++;
            if (count == n) {
                return (seq_t)p;
            }
        }
        p += 2;
    }
}
