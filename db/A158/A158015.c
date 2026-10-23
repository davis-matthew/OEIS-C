/*
 * A158015  Primes p such that 6*p-1 is also prime.
 * https://oeis.org/A158015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 294, u32 n <= 370, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes p such that 6*p-1 is also prime */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    uint64_t p = 1;          /* start search before the first prime */
    idx_t   count = 0;       /* number of qualifying primes found */

    while (true) {
        p = next_prime(p);   /* next prime after current p */
        seq_t ps   = (seq_t)p;
        seq_t cand = ps * 6;
        cand -= 1;

        if (is_prime_big(cand)) {
            ++count;
            if (count == n)
                return ps;  /* return the prime p */
        }
    }
}
