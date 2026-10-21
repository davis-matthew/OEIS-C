/*
 * A075815  Palindromic odd composite numbers with an odd number of prime
 *          factors (counted with multiplicity).
 * https://oeis.org/A075815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 141, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A075815: Palindromic odd composite numbers with an odd number
   of prime factors (counted with multiplicity).  The sequence is generated
   by enumerating palindromes in ascending order and keeping those that are
   odd, composite, and have an odd big‑omega.  Results are cached so that
   calls may arrive in any order. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/count_prime_factors.h"

/* Powers of ten up to 10^19 (enough for any 64‑bit value).  Used to
   initialise palindrome half ranges. */
static const uint64_t pow10[20] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL
};

/* Build the next palindrome in the current length class */
static uint64_t make_palindrome(uint64_t half, uint64_t len)
{
    uint64_t r = half;
    uint64_t x = half;
    if (len & 1U)            /* skip middle digit for odd length */
        x /= 10;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

/* Main sequence function.  It keeps a cache of already‑computed values
   and a simple generator that walks through palindromes in order. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Generator state: length of current palindrome, half length,
       and current prefix (half) for that length. */
    static uint64_t gen_len = 0;           /* current palindrome length */
    static uint64_t gen_half_len = 0;      /* number of digits in half */
    static uint64_t gen_half_start = 0;    /* smallest half for this length */
    static uint64_t gen_half_end   = 0;    /* largest half for this length */
    static uint64_t gen_half       = 0;    /* current half */

    /* If we already have the requested term, return it. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Initialise generator on first call. */
    if (gen_len == 0) {
        gen_len = 1;
        gen_half_len = (gen_len + 1) / 2;
        gen_half_start = pow10[gen_half_len - 1];
        gen_half_end   = pow10[gen_half_len] - 1;
        gen_half       = gen_half_start;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        uint64_t cand = make_palindrome(gen_half, gen_len);

        /* Advance to next half.  If we've exhausted the current length,
           move to the next length. */
        ++gen_half;
        if (gen_half > gen_half_end) {
            ++gen_len;
            gen_half_len = (gen_len + 1) / 2;
            gen_half_start = pow10[gen_half_len - 1];
            gen_half_end   = pow10[gen_half_len] - 1;
            gen_half       = gen_half_start;
        }

        /* Apply the required filters. */
        if ((cand & 1ULL) == 0)            /* must be odd */
            continue;
        uint64_t omega = (uint64_t)count_prime_factors((seq_t)cand);
        if (omega <= 1)                    /* must be composite (omega>1) */
            continue;
        if ((omega & 1ULL) == 0)           /* odd number of prime factors */
            continue;

        /* Store the candidate in the cache. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!newp) exit(1);          /* abort on allocation failure */
            if (cache) {
                for (size_t i = 0; i < cache_len; ++i)
                    newp[i] = cache[i];
                free(cache);
            }
            cache = newp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)cand;
    }

    return cache[n - 1];
}
