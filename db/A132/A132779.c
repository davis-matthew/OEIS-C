/*
 * A132779  Primes p such that 33 does not divide p^n - 2 for any n.
 * https://oeis.org/A132779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:
   a(n) = the n-th prime p such that 33 does not divide p^k - 2 for any k ≥ 1.
   This is equivalent to p mod 33 belonging to
   {1,3,4,5,7,10,11,13,14,16,19,20,23,25,26,28,31,32}. */

/*  Include what the build system provides for seq_t, idx_t, and standard types.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*  Cache of already computed terms  */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/*  Cache of primes discovered so far, used for trial division  */
static idx_t *primes_all = NULL;
static size_t primes_all_len = 0;
static size_t primes_all_cap = 0;

/*  Residues modulo 33 that are allowed  */
static const bool allowed[33] = {
    /* 0 */ false,
    /* 1 */ true,
    /* 2 */ false,
    /* 3 */ true,
    /* 4 */ true,
    /* 5 */ true,
    /* 6 */ false,
    /* 7 */ true,
    /* 8 */ false,
    /* 9 */ false,
    /*10 */ true,
    /*11 */ true,
    /*12 */ false,
    /*13 */ true,
    /*14 */ true,
    /*15 */ false,
    /*16 */ true,
    /*17 */ false,
    /*18 */ false,
    /*19 */ true,
    /*20 */ true,
    /*21 */ false,
    /*22 */ false,
    /*23 */ true,
    /*24 */ false,
    /*25 */ true,
    /*26 */ true,
    /*27 */ false,
    /*28 */ true,
    /*29 */ false,
    /*30 */ false,
    /*31 */ true,
    /*32 */ true
};

/*  Ensure space for at least 'num' primes  */
static void expand_primes_all(void)
{
    if (primes_all_len >= primes_all_cap) {
        size_t new_cap = (primes_all_cap == 0) ? 64 : primes_all_cap * 2;
        idx_t *tmp = (idx_t *)realloc(primes_all, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        primes_all = tmp;
        primes_all_cap = new_cap;
    }
}

/*  Ensure space for at least 'num' cached terms  */
static void expand_cache(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/*  Trial‑division primality test using the cached primes  */
static bool is_prime_candidate(idx_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_all_len; ++i) {
        idx_t p = primes_all[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/*  Main sequence function  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for n=0, just return 0 */

    /*  Grow the cache until it contains at least n terms  */
    while (cache_len < n) {
        static idx_t candidate = 2;        /* remembers last searched value */
        idx_t p = candidate;
        bool found = false;
        while (!found) {
            /* Skip even numbers except 2 */
            if ((p & 1) == 0) {
                if (p != 2) {
                    p++;
                    continue;
                }
                /* p == 2: allow to test for primality */
            }

            /* Test for primality and update caches */
            if (is_prime_candidate(p)) {
                expand_primes_all();
                primes_all[primes_all_len++] = p;

                if (allowed[p % 33]) {
                    expand_cache();
                    cache[cache_len++] = (seq_t)p;
                }
                found = true;
            }

            /* Advance candidate for next trial */
            p++;
        }

        /* Next search starts just after the prime we just processed */
        candidate = p;
    }

    return cache[n - 1];
}
