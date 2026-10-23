/*
 * A038918  Primes p such that 39 is not a square mod p.
 * https://oeis.org/A038918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p for which the Jacobi symbol (39 | p) =
 *            –1, i.e. 39 is a quadratic non‑residue modulo p. The primes 2,3
 *            and 13 are excluded because 39 ≡ 0 (mod p). This is the
 *            definition of OEIS A038918
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a small on‑demand sieve and stores the found primes
   in a cache so that calls with arbitrary order of n are fast.  */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------------ */
/*  Cache for the computed values                                        */
/* ------------------------------------------------------------------ */
static seq_t *cache_vals = NULL;   /* array of a(k) values */
static size_t cache_len  = 0;      /* number of cached values   */
static size_t cache_cap  = 0;      /* capacity of cache_vals    */

/* ------------------------------------------------------------------ */
/*  Prime sieve management                                               */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list = NULL;  /* array of primes from sieve */
static size_t    prime_cnt   = 0;    /* number of primes in array   */
static size_t    prime_idx   = 0;    /* next prime to examine       */
static idx_t     prime_limit = 0;    /* current sieve upper bound   */

/* ------------------------------------------------------------------ */
/*  Utility: grow cache to hold at least 'needed' entries              */
/* ------------------------------------------------------------------ */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache_vals, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In real code we would abort or handle the error; here we simply
           leave the allocation zeroed to keep the function legal.        */
        return;
    }
    cache_vals = tmp;
    cache_cap  = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Rebuild the prime list up to a new upper bound                      */
/* ------------------------------------------------------------------ */
static void rebuild_primes(idx_t new_limit)
{
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }
    build_primes_upto(new_limit, (uint64_t **)&prime_list, &prime_cnt);
    prime_limit = new_limit;
    prime_idx   = 0;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for offset 1, return 0 harmlessly */
        return (seq_t)0;

    /* Fill the cache until it contains at least n entries */
    while (cache_len < (size_t)n) {
        /* If we have no primes yet, start with a reasonable limit */
        if (prime_list == NULL) {
            /* 2 million is plenty for n up to ~20000 in practice */
            rebuild_primes(2000000);
        }

        /* Scan remaining primes until we accumulate enough results */
        while (prime_idx < prime_cnt) {
            uint64_t p = prime_list[prime_idx++];
            if (p <= 2)             /* skip 2 */
                continue;
            if (p == 3 || p == 13)  /* 39 ≡ 0 (mod p) --> not a quadratic residue */
                continue;
            if (jacobi_symbol(39ULL, p) == -1) {
                grow_cache(cache_len + 1);
                cache_vals[cache_len++] = (seq_t)p;
                if (cache_len >= (size_t)n)
                    break;
            }
        }

        /* If we ran out of primes, extend the sieve and continue */
        if (cache_len < (size_t)n) {
            idx_t new_limit = prime_limit * 2;   /* double the bound */
            rebuild_primes(new_limit);
        }
    }

    return cache_vals[n - 1];
}
