/*
 * A218833  Primes p such that 2*p is greater than the greatest prime factor
 *          of p^2 - 1 and p^2 + 1.
 * https://oeis.org/A218833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation: primes p such that 2*p is greater than the largest prime
 * factor of both p² − 1 and p² + 1.
 * Returns the n‑th such prime (a(1) = 3).
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_vals = NULL;          /* list of accepted primes           */
static size_t seq_len = 0, seq_cap = 0;
static uint64_t *prime_list = NULL;     /* list of all primes found          */
static size_t prime_len = 0, prime_cap = 0;
static uint64_t next_candidate = 3;     /* next odd candidate to test       */

/*  Return true iff all prime factors of p²+1 are ≤ 2*p.
 *  The factorisation is performed with the primes already generated.
 */
static bool good_prime(uint64_t p)
{
    seq_t x = (seq_t)p * (seq_t)p + 1;   /* p² + 1   */
    seq_t bound = (seq_t)p << 1;         /* 2*p      */
    seq_t rem = x;

    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t q = prime_list[i];
        seq_t sq = (seq_t)q * (seq_t)q;
        if (sq > rem) break;            /* no more small divisors          */
        while (rem % q == 0) {
            rem /= q;
        }
    }
    /* if after removing small primes rem > 1, it is a prime > sqrt(p²+1) */
    return !(rem > bound);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for offset 0 */

    /* initialise static storage once */
    if (prime_list == NULL) {
        prime_cap = 64;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_list[0] = 2;             /* seed with 2 */
        prime_len = 1;
    }
    if (seq_vals == NULL) {
        seq_cap = 64;
        seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* gather primes until we have enough terms */
    while (seq_len < n) {
        uint64_t cand = next_candidate;
        next_candidate += 2;           /* skip even numbers */

        /* primality test with trial division */
        bool is_prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            if ((unsigned __int128)p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (!is_prime) continue;

        /* store new prime */
        if (prime_len == prime_cap) {
            prime_cap *= 2;
            prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        }
        prime_list[prime_len++] = cand;

        /* check the defining property */
        if (good_prime(cand)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
            }
            seq_vals[seq_len++] = (seq_t)cand;
        }
    }
    return seq_vals[n - 1];
}
