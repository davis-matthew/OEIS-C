/*
 * A122487  2 together with odd primes p that divide Fibonacci[(p+1)/2].
 * https://oeis.org/A122487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime p (1‑based) such that p | F[(p+1)/2]
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts with 2,13,17,37,53,… (OEIS A122487) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */

/* Fast‑doubling Fibonacci modulo mod.
   Computes F(n) mod mod for n >= 1. */
static uint64_t fib_mod_u64(idx_t n, uint64_t mod)
{
    if (mod == 1)                /* everything is 0 mod 1 */
        return 0;

    uint64_t a = 0;              /* F(0) */
    uint64_t b = 1;              /* F(1) */
    int msb = 63 - __builtin_clzll((unsigned long long)n); /* highest set bit */

    for (int i = msb; i >= 0; --i) {
        /* t1 = F(2k) = F(k) * (2*F(k+1) - F(k))   */
        /* t2 = F(2k+1) = F(k)^2 + F(k+1)^2        */
        uint64_t two_b  = (__int128)2 * b % mod;
        uint64_t t1_val = (__int128)a * ((two_b + mod - a) % mod) % mod;
        uint64_t t2_val = (( (__int128)a * a ) % mod + ( (__int128)b * b ) % mod) % mod;

        a = t1_val;
        b = t2_val;

        if ((n >> i) & 1ULL) {
            /* move to the next Fibonacci pair */
            uint64_t c = (a + b) % mod;   /* F(n+1) */
            a = b;
            b = c;
        }
    }
    return a;                      /* F(n) mod mod */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined: offset starts at 1 */

    /* static data for memoisation */
    static uint64_t *cache = NULL;
    static size_t   count = 0;
    static size_t   cap   = 0;
    static uint64_t last_checked = 2;   /* next integer to probe for primality */
    static bool initialized = false;

    if (!initialized) {           /* first call initialisation */
        cap   = 4;
        cache = (uint64_t *)malloc(cap * sizeof(uint64_t));
        if (!cache) abort();       /* allocation failure */
        initialized = true;
    }

    /* continue generating until we have the n‑th element */
    while (count < n) {
        uint64_t cand;

        /* obtain next candidate prime */
        if (count == 0) {
            cand = 2;               /* the first element is always 2 */
        } else {
            cand = next_prime(last_checked);
        }
        last_checked = cand + 1;     /* advance for the next round */

        /* test the condition only for odd primes */
        if (cand != 2) {
            idx_t k = (cand + 1) / 2;            /* the index of Fibonacci to test */
            if (fib_mod_u64(k, cand) != 0) {      /* not divisible, skip */
                continue;
            }
        }

        /* candidate passes, store it */
        if (count >= cap) {                  /* grow cache */
            cap <<= 1;
            cache = (uint64_t *)realloc(cache, cap * sizeof(uint64_t));
            if (!cache) abort();
        }
        cache[count++] = cand;
    }

    return (seq_t)cache[n - 1];
}
