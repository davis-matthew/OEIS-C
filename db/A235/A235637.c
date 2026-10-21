/*
 * A235637  Primes whose base-7 representation also is the base-6
 *          representation of a prime.
 * https://oeis.org/A235637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 490, u32 n <= 609, u64 n <= 9255,
 *            u128 n <= 9255, big any n
 * Helpers:   append_candidate, build_primes, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find primes whose base‑7 representation uses only digits 0–5,
   and when that same digit sequence is interpreted as a base‑6
   number it is also prime. */

/* The implementation is a straightforward enumerator:
   1.  Generate primes incrementally using build_primes.
   2.  For each prime, convert its decimal representation to base‑7.
       Reject it if any digit ≥ 6 (since base‑6 digits must be < 6).
   3.  Interpret the accepted digit sequence in base‑6 to obtain q.
   4.  Test q for primality with is_prime_big.
   5.  Store qualifying primes in a static cache so that arbitrary
       order of calls is supported. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/build_primes.h"
#include "../utils/append_candidate.h"

/* static cache of found terms */
static seq_t *cache = NULL;   /* cached sequence values */
static size_t cache_len = 0;  /* how many values are cached */
static size_t cache_cap = 0;  /* capacity of the cache */

/* Test whether a prime satisfies the base-7 → base-6 property. */
static bool base7_to_base6_prime(seq_t p)
{
    seq_t q = 0;     /* value when digits are interpreted in base 6 */
    seq_t mul = 1;   /* current power of 6 */

    while (p > 0) {
        seq_t d = p % 7;
        /* Reject if digit would be invalid in base‑6. */
        if (d >= 6)
            return false;
        q += d * mul;
        mul *= 6;    /* safe for the expected range of primes  */
        p /= 7;
    }

    return is_prime_big(q);
}

/* Extend the cache until it contains at least n_needed terms. */
static void generate_upto(size_t n_needed)
{
    /* static state for incremental sieve */
    static idx_t sieve_limit = 0;      /* highest number sieved so far */
    static idx_t prime_start = 0;     /* index of first prime not yet processed */

    while (cache_len < n_needed) {
        /* Initialize or double the sieve limit. */
        if (sieve_limit == 0)
            sieve_limit = 1000000;
        else
            sieve_limit *= 2;

        /* Build all primes up to the new limit. */
        idx_t *prime_list = NULL;
        size_t prime_cnt = 0;
        build_primes(sieve_limit, &prime_list, &prime_cnt);

        /* Examine only the freshly added primes. */
        for (size_t i = prime_start; i < prime_cnt; ++i) {
            seq_t p = (seq_t)prime_list[i];
            if (base7_to_base6_prime(p))
                append_candidate(&cache, &cache_len, &cache_cap, p);
        }

        /* Clean up. */
        free(prime_list);
        prime_start = prime_cnt;
    }
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* undefined for n < 1, but harmless */

    if (cache_len < (size_t)n)
        generate_upto((size_t)n);

    return cache[n - 1];       /* a(1) corresponds to index 0 */
}
