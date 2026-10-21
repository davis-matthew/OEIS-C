/*
 * A062251  Take minimal prime q such that n(q+1)-1 is prime (A060324), that
 *          is, the smallest prime q so that n = (p+1)/(q+1) with p prime;
 *          sequence gives values of p.
 * https://oeis.org/A062251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1331, u32 n <= 3666, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (q_min(n)+1)*n - 1
 * where q_min(n) is the smallest prime q such that n*(q+1)-1 is prime.
 * The function searches primes q starting from 2 and returns the first
 * p = n*(q+1)-1 that is prime.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == 0u) {
        /* Offset starts at 1; undefined for n=0. */
        return 0;
    }

    uint64_t q = 2u;          /* start with the first prime */
    for (;;) {
        seq_t p = (seq_t)n * ((seq_t)q + 1u) - 1u;
        if (is_prime_big(p))
            return p;
        ++q;
        while (!is_prime_small(q))
            ++q;
    }
}
