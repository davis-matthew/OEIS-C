/*
 * A155916  Prime factors of odd Fibonacci numbers.
 * https://oeis.org/A155916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 4085, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, jacobi_symbol, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155916: Prime factors of odd Fibonacci numbers.
   a(n) = the n‑th prime p such that the rank of appearance r(p)
   of p in the Fibonacci sequence is NOT divisible by 3. */

/* Standard includes */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Utility functions supplied by the build */
#include "../utils/is_prime.h"
#include "../utils/jacobi_symbol.h"
#include "../utils/seq_realloc.h"

/* --------------------------------------------------------------------- */
/*  Fast doubling algorithm for Fibonacci numbers modulo a prime p     */
/* --------------------------------------------------------------------- */
static void fib_pair(uint64_t p, uint64_t n, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    uint64_t a, b;
    fib_pair(p, n >> 1, &a, &b);
    /* compute c = F(2k), d = F(2k+1) */
    uint64_t two_b = ((unsigned __int128)b * 2) % p;
    uint64_t two_b_minus_a = (two_b + p - a) % p;
    uint64_t c = ((unsigned __int128)a * two_b_minus_a) % p;
    uint64_t d = ((unsigned __int128)a * a + (unsigned __int128)b * b) % p;
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % p;
    }
}
static uint64_t fib_mod(uint64_t p, uint64_t n)
{
    uint64_t fn, _;
    fib_pair(p, n, &fn, &_);
    return fn;
}

/* --------------------------------------------------------------------- */
/*  Determine whether a prime p belongs to the sequence                */
/* --------------------------------------------------------------------- */
static bool include_prime(uint64_t p)
{
    if (p == 5)                      /* 5 divides F_5 (odd) */
        return true;

    int leg = jacobi_symbol(5, p);   /* Legendre symbol (5/p) */
    uint64_t m = (leg == 1) ? p - 1 : p + 1;   /* r(p) | m */

    /* factor m into distinct primes */
    uint64_t tmp = m;
    uint64_t fac[64];
    int fc = 0;
    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            fac[fc++] = d;
            while (tmp % d == 0)
                tmp /= d;
        }
    }
    if (tmp > 1)
        fac[fc++] = tmp;

    /* compute minimal rank of appearance r(p) */
    uint64_t r = m;
    for (int i = 0; i < fc; ++i) {
        uint64_t q = fac[i];
        while (r % q == 0) {
            uint64_t cand = r / q;
            if (fib_mod(p, cand) == 0)
                r = cand;
            else
                break;
        }
    }
    return (r % 3) != 0;
}

/* --------------------------------------------------------------------- */
/*  Cache for printed terms                                               */
/* --------------------------------------------------------------------- */
static seq_t  *cache      = NULL;
static size_t cache_cap   = 0;
static size_t cache_len   = 0;
static uint64_t next_candidate = 3;   /* first odd prime */

/* --------------------------------------------------------------------- */
/*  Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    while (cache_len < (size_t)n) {
        /* Find the next prime */
        while (!is_prime(next_candidate))
            next_candidate += 2;          /* skip evens */
        uint64_t p = next_candidate;
        next_candidate += 2;             /* prepare for next search */

        if (include_prime(p)) {          /* add to cache */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap) ? (cache_cap << 1) : 128;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) /* allocation failure – graceful exit */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p;
        }
    }
    return cache[(size_t)n - 1];
}
