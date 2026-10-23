/*
 * A191087  Primes p that have Kronecker symbol (p|95) = -1.
 * https://oeis.org/A191087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p with Kronecker symbol (p|95) = -1,
   i.e. (p|5) * (p|19) = -1.  The function builds a cache of such primes
   incrementally, so it can answer any query a(n) quickly. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"

/* Prime list ------------------------------------------------------------ */
static uint64_t *prime_list = NULL;
static size_t   prime_count = 0;
static size_t   prime_cap   = 0;

/* Sequence cache -------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;

/* Number to test next (odd) --------------------------------------------- */
static uint64_t next_candidate = 3;  /* 2 is handled separately */

/* Flag that indicates whether the tables have been initialised ----------- */
static bool initialized = false;

/* ------------------------------------------------------------------------- */
static void add_prime(uint64_t p)
{
    if (prime_count == prime_cap) {
        prime_cap = prime_cap == 0 ? 16 : prime_cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
    }
    prime_list[prime_count++] = p;
}

/* ------------------------------------------------------------------------- */
static void add_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
    }
    cache[cache_len++] = val;
}

/* ------------------------------------------------------------------------- */
static bool passes(uint64_t p)
{
    int s5  = jacobi_symbol(p, 5);
    int s19 = jacobi_symbol(p, 19);
    /* j.s. returns 0 iff p | n; for prime p not equal to 5 or 19 this cannot
       happen, but we guard against it anyway. */
    if (s5 == 0 || s19 == 0) return false;
    return (s5 * s19) == -1;
}

/* ------------------------------------------------------------------------- */
static void initialise(void)
{
    if (initialized) return;
    initialised: ;
    /* Add the smallest prime, 2 */
    add_prime(2);
    if (passes(2)) add_cache(2);
    next_candidate = 3;
    initialized = true;
}

/* ------------------------------------------------------------------------- */
static void generate_until(idx_t n)
{
    initialise();

    while (cache_len < (size_t)n) {
        uint64_t p = next_candidate;
        bool is_prime = true;

        /* Test primality by trial division using primes found so far */
        if (p == 2) {
            is_prime = true;
        } else if (p % 2 == 0) {
            is_prime = false;
        } else {
            for (size_t i = 0; i < prime_count; ++i) {
                uint64_t q = prime_list[i];
                if (q * q > p) break;
                if (p % q == 0) { is_prime = false; break; }
            }
        }

        next_candidate += 1;  /* advance for next time */
        if (!is_prime) continue;

        /* p is prime */
        add_prime(p);
        if (passes(p)) add_cache(p);
    }
}

/* ------------------------------------------------------------------------- */
/* External user function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1 */
    generate_until(n);
    return cache[n - 1];
}
