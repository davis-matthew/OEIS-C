/*
 * A019365  Primes with primitive root 40.
 * https://oeis.org/A019365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p (p ≥ 7) such that 40 is a primitive root
 *            modulo p. The implementation enumerates primes, tests the
 *            primitive root condition by modular exponentiation and trial
 *            division using a pre–computed list of small primes, and caches
 *            the results so later calls are fast
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, mod_pow, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/mod_pow.h"
#include "../utils/is_prime_big.h"
#include "../utils/sieve_primes.h"

/* Small primes up to 1 000 000 are used for factorisation of p‑1. */
static idx_t *prime_factors = NULL;
static size_t prime_factors_cnt = 0;

static void init_prime_factors(void)
{
    if (prime_factors == NULL)
    {
        sieve_primes((idx_t)1000000, &prime_factors, &prime_factors_cnt);
    }
}

/* Return true iff 40 is a primitive root modulo p.  p is a prime > 5. */
static bool is_primitive_root_40(seq_t p)
{
    if (p <= (seq_t)5) return false;

    seq_t orig_m = p - (seq_t)1;   /* m = p‑1 */
    seq_t mtmp   = orig_m;

    init_prime_factors();

    for (size_t i = 0; i < prime_factors_cnt; ++i)
    {
        idx_t   q  = prime_factors[i];
        seq_t  qsq = (seq_t)q * (seq_t)q;
        if (qsq > mtmp) break;                 /* no more small factors */

        if (mtmp % (seq_t)q == 0)
        {
            uint64_t exp = (uint64_t)(orig_m) / (uint64_t)q;
            if (mod_pow((seq_t)40, exp, p) == (seq_t)1) return false;
            while (mtmp % (seq_t)q == 0)
                mtmp /= (seq_t)q;
        }
    }

    if (mtmp > (seq_t)1)                      /* remaining prime factor */
    {
        uint64_t exp = (uint64_t)(orig_m) / (uint64_t)mtmp;
        if (mod_pow((seq_t)40, exp, p) == (seq_t)1) return false;
    }

    return true;
}

/* Cache for sequence values. */
static seq_t *cache      = NULL;
static size_t cache_cnt  = 0;
static size_t cache_cap  = 0;

static void cache_push(seq_t val)
{
    if (cache_cnt == cache_cap)
    {
        cache_cap = cache_cap ? cache_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_cnt++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* sequence defined for n ≥ 1 */

    static seq_t cand = (seq_t)2;          /* next candidate to test */

    while (cache_cnt < (size_t)n)
    {
        cand += (seq_t)1;                  /* increase candidate */

        if (!is_prime_big(cand)) continue; /* skip non‑primes */

        if (is_primitive_root_40(cand))
            cache_push(cand);
    }

    return cache[(size_t)n - 1];
}
