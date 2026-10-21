/*
 * A059315  Primes p such that x^28 = 2 has no solution mod p.
 * https://oeis.org/A059315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation for OEIS A059315: primes p such that the congruence
   x^28 ≡ 2 (mod p) has no solution.  The routine checks whether 2
   is a 28th‑power residue modulo p by evaluating
   2^((p‑1)/gcd(28,p‑1)) mod p – if the result is 1, a solution exists.
   The sequence is generated on demand and cached. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------------ */
/*  Static state -- a fixed size cache of sequence values              */
/*  ------------------------------------------------------------------ */
#define CACHE_CAPACITY 200000   /* should accommodate the first 10 000 terms */

static seq_t  cache[CACHE_CAPACITY];
static size_t cache_size = 0;          /* number of cached values */

/* ------------------------------------------------------------------ */
/*  Prime handling – a dynamic array of primes up to a limit           */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list = NULL;     /* array of primes */
static size_t   prime_cnt  = 0;         /* number of primes in the array */
static size_t   prime_index = 0;        /* next prime to test */
static uint64_t prime_sieve_limit = 100000;   /* current upper bound for sieve */

/* ------------------------------------------------------------------ */
/*  Helper:  does prime p satisfy the congruence?                     */
/* ------------------------------------------------------------------ */
static bool has_solution(uint64_t p)
{
    seq_t mod  = (seq_t)p;
    seq_t base = (seq_t)2;
    seq_t g    = gcd((seq_t)28, (seq_t)(p - 1));
    uint64_t d = (uint64_t)g;                 /* exponent divisor            */
    seq_t exp  = (seq_t)((p - 1) / d);        /* exponent for power test     */
    seq_t res  = mod_pow(base, (uint64_t)exp, mod);
    return res == (seq_t)1;
}

/* ------------------------------------------------------------------ */
/*  Fill the cache until it contains at least `need` terms.           */
/*  If primes are exhausted the sieve limit is doubled and new       */
/*  primes are generated.  The cache array is fixed size; the build   */
/*  guarantees that CACHE_CAPACITY is large enough for requested      */
/*  indices (the sequence grows slower than the primes).             */
/* ------------------------------------------------------------------ */
static void extend_cache(size_t need)
{
    /* initial sieve if necessary */
    if (prime_list == NULL) {
        build_primes_upto(prime_sieve_limit, &prime_list, &prime_cnt);
    }

    while (cache_size < need) {
        /* enlarge sieve if we have inspected all current primes */
        if (prime_index >= prime_cnt) {
            prime_sieve_limit *= 2;
            free(prime_list);
            build_primes_upto(prime_sieve_limit, &prime_list, &prime_cnt);
            /* prime_index remains unchanged – we will continue from where we paused */
        }

        uint64_t p = prime_list[prime_index++];
        if (p <= 2) continue;  /* skip 2 – the sequence starts with 3 */

        if (!has_solution(p)) {
            if (cache_size < CACHE_CAPACITY) {
                cache[cache_size++] = (seq_t)p;
            } else {
                /* If we really exceed capacity, abort – this should not happen */
                return;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Public function: a(idx_t n) – returns the n‑th term                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1, but safe */
    extend_cache((size_t)n);
    return cache[(size_t)(n - 1)];
}
