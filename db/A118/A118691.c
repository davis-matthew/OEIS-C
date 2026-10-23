/*
 * A118691  Semiprimes for which the digital root is also a semiprime.
 * https://oeis.org/A118691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 3432, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118691: semiprimes whose digital root is also a semiprime.
   Find the n‑th term by brute‑force search. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* Test whether a 64‑bit integer N is a semiprime (product of exactly two
   primes, counting multiplicity).  Uses a precomputed list of primes. */
static bool is_semiprime(uint64_t N, const uint64_t *primes, size_t primes_cnt) {
    if (N < 4) return false;
    int factors = 0;
    uint64_t tmp = N;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        while (tmp % p == 0) {
            ++factors;
            if (factors > 2) return false;
            tmp /= p;
        }
    }
    if (tmp > 1) ++factors;
    return factors == 2;
}

/* Compute the digital root of a non‑negative number. */
static seq_t digital_root(seq_t x) {
    /* digital root = 1 + (x-1) % 9, except when x==0. */
    if (x == (seq_t)0) return (seq_t)0;
    uint64_t mod9 = (uint64_t)(x % (seq_t)9);
    return (mod9 == 0) ? (seq_t)9 : (seq_t)mod9;
}

/* External function required by the test harness. */
seq_t a(idx_t n) {
    static uint64_t *prime_list = NULL;    /* array of primes */
    static size_t prime_cnt = 0;           /* number of primes */
    static bool primes_initialized = false;

    /* Initialise the prime list once.  A bound of 100 000 gives plenty of
       primes for generating semiprimes up to the 10 000th term. */
    if (!primes_initialized) {
        idx_t limit = (idx_t)100000;
        build_primes_upto(limit, &prime_list, &prime_cnt);
        primes_initialized = true;
    }

    uint64_t found = 0;            /* how many qualifying semiprimes found */
    uint64_t candidate = 3;        /* start just below the first semiprime */
    seq_t answer = (seq_t)0;       /* value to return */

    while (found < n) {
        ++candidate;
        /* Candidate must be at least 4 to be a semiprime. */
        if (candidate < 4) continue;

        if (is_semiprime(candidate, prime_list, prime_cnt)) {
            uint64_t dr = candidate % 9;
            if (dr == 0) dr = 9;                /* digital root of a multiple of 9 */
            /* The only semiprime digital roots are 4, 6, and 9. */
            if (dr == 4 || dr == 6 || dr == 9) {
                ++found;
                answer = (seq_t)candidate;
            }
        }
    }
    return answer;
}
