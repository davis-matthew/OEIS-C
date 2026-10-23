/*
 * A023216  Primes p such that 4*p + 9 is also prime.
 * https://oeis.org/A023216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1541, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: returns the n‑th prime p such that 4·p+9 is also prime. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset 1; value for n < 1 is undefined */

    idx_t candidate = 2;
    idx_t count = 0;

    for (;;) {
        if (is_prime(candidate)) {
            idx_t q = 4 * candidate + 9;
            if (is_prime(q)) {
                ++count;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
