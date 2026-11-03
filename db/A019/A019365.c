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
 * Validated: standard, pass, 72 terms through n = 919 (2026-11-03)
 * Helpers:   is_prime_u64, mem, powmod, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <string.h>
#include "../utils/powmod.h"
#include "../utils/sieve_primes.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
            if (powmod((seq_t)40, exp, p) == (seq_t)1) return false;
            while (mtmp % (seq_t)q == 0)
                mtmp /= (seq_t)q;
        }
    }

    if (mtmp > (seq_t)1)                      /* remaining prime factor */
    {
        uint64_t exp = (uint64_t)(orig_m) / (uint64_t)mtmp;
        if (powmod((seq_t)40, exp, p) == (seq_t)1) return false;
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
        cache = mem_resize(seq_t, cache, cache_cap);
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
