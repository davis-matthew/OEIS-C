/*
 * A106069  Primes p such that 3*p + 8 and 8*p + 3 are primes.
 * https://oeis.org/A106069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106069: primes p such that 3p+8 and 8p+3 are primes. */
/* Implementation: search over primes, testing the two linear forms. */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t found = 0;
    uint64_t p = 2;

    while (1) {
        if (is_prime(p)) {
            unsigned __int128 t1 = (unsigned __int128)3 * p + 8;
            unsigned __int128 t2 = (unsigned __int128)8 * p + 3;

            if (t1 <= UINT64_MAX && t2 <= UINT64_MAX) {
                if (is_prime((uint64_t)t1) && is_prime((uint64_t)t2)) {
                    ++found;
                    if (found == n)
                        return (seq_t)p;
                }
            }
        }

        if (p == 2)
            p = 3;
        else
            p += 2;   /* skip even numbers */
    }
}
