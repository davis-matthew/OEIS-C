/*
 * A006512  Greater of twin primes.
 * https://oeis.org/A006512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 860, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9665 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_u64.h"

/* Search: find the n-th prime p where p-2 is also prime */

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t p = 5;
    
    while (1) {
        if (is_prime_u64((uint64_t)p) && is_prime_u64((uint64_t)(p - 2))) {
            count++;
            if (count == (seq_t)n) {
                return p;
            }
        }
        p += 2;
    }
}
