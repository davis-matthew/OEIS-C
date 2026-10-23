/*
 * A025022  Numbers whose least quadratic nonresidue (A020649) is 5.
 * https://oeis.org/A025022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by searching for numbers n such that the least
 *            quadratic nonresidue (A020649) is 5. For each candidate n we
 *            test that 2,3,4 are quadratic residues modulo n and 5 is not.
 *            The tests are carried out using prime factorisation and
 *            elementary facts about quadratic residues modulo prime powers
 * Range:     u8 n <= 10, u16 n <= 1629, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* --------------------------------------------------------------------
   JACOBI SYMBOL
   -------------------------------------------------------------------- */
#include "../utils/jacobi_symbol.h"

/* --------------------------------------------------------------------
   PRIMES SIEVE (up to 50000 for factorisation)
   -------------------------------------------------------------------- */
#define PRIME_LIMIT 50000
static uint32_t prime_table[PRIME_LIMIT / 2];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt != 0) return;                /* already done       */
    bool is_composite[PRIME_LIMIT] = {false};
    for (size_t i = 2; i < PRIME_LIMIT; ++i)
    {
        if (!is_composite[i])
        {
            prime_table[prime_cnt++] = (uint32_t)i;
            if (i * i < PRIME_LIMIT)
                for (size_t j = i * i; j < PRIME_LIMIT; j += i)
                    is_composite[j] = true;
        }
    }
}

/* --------------------------------------------------------------------
   COUNT TRAILING ZEROS (v2)
   -------------------------------------------------------------------- */
static inline uint32_t v2(uint64_t x)
{
    uint32_t c = 0;
    while ((x & 1ULL) == 0) { x >>= 1; ++c; }
    return c;
}

/* --------------------------------------------------------------------
   RESIDUE TEST FOR 2^e
   -------------------------------------------------------------------- */
static bool residue_2pow(int e, uint64_t k)
{
    if (k == 0) return true;
    if (e == 0) return false;             /* not used              */
    if (k & 1ULL) {                       /* k odd                  */
        if (e == 1) return true;          /* mod 2                  */
        if (e == 2) return (k % 4ULL == 1ULL);
        return (k % 8ULL == 1ULL);        /* e >= 3                 */
    } else {                               /* k even                  */
        uint32_t t = v2(k);
        if (t >= (uint32_t)e) return true;
        if (t & 1)           return false;
        uint64_t kodd = k >> t;
        int eprime = e - t;
        if (eprime == 0) return true;
        if (eprime == 1) return true;
        if (eprime == 2) return (kodd % 4ULL == 1ULL);
        return (kodd % 8ULL == 1ULL);
    }
}

/* --------------------------------------------------------------------
   RESIDUE TEST FOR PRIME POWER p^e
   -------------------------------------------------------------------- */
static bool residue_prime_power(uint64_t p, int e, uint64_t k)
{
    if (k == 0) return true;
    if (p == 2)   return residue_2pow(e, k);

    if (k % p == 0) {
        uint64_t tmp = k;
        for (int i = 0; i < e; ++i) {
            if (tmp % p != 0) return false;
            tmp /= p;
        }
        return true;                    /* fully divisible */
    }

    /* k not divisible by p: use Legendre symbol */
    return jacobi_symbol(k % p, p) == 1;
}

/* --------------------------------------------------------------------
   FACTORISATION OF n INTO (prime, exponent) arrays
   -------------------------------------------------------------------- */
#define MAX_FACTORS 12
static void factorize(uint64_t n, uint64_t pf[], int exp[],
                      size_t *cnt)
{
    init_primes();
    size_t c = 0;
    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_table[i];
        if ((uint64_t)p * (uint64_t)p > m) break;
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            pf[c]   = p;
            exp[c]  = e;
            ++c;
        }
    }
    if (m > 1) {
        pf[c]   = m;
        exp[c]  = 1;
        ++c;
    }
    *cnt = c;
}

/* --------------------------------------------------------------------
   TEST IF k IS A QUADRATIC RESIDUE MOD n USING PRE-FACTORED PRIMES
   -------------------------------------------------------------------- */
static bool test_factors(const uint64_t pf[], const int exp[],
                         size_t cnt, uint64_t k)
{
    for (size_t i = 0; i < cnt; ++i)
        if (!residue_prime_power(pf[i], exp[i], k))
            return false;
    return true;
}

/* --------------------------------------------------------------------
   CACHE HANDLING
   -------------------------------------------------------------------- */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;
static void grow_cache(void)
{
    size_t newcap = (cache_cap == 0 ? 16 : cache_cap << 1);
    seq_t *p = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!p) { free(cache); abort(); }
    cache = p;
    cache_cap = newcap;
}

static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) grow_cache();
    cache[cache_len++] = val;
}

/* ====================================================================
   PUBLIC FUNCTION
   ==================================================================== */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but safe */

    /* extend cache if necessary */
    while (cache_len < n) {
        static uint64_t cand = 2;
        /* factorise cand once */
        uint64_t pf[MAX_FACTORS];
        int exp[MAX_FACTORS];
        size_t cnt;
        factorize(cand, pf, exp, &cnt);

        bool ok = true;
        /* 2,3,4 must be residues */
        if (!test_factors(pf, exp, cnt, 2)) ok = false;
        else if (!test_factors(pf, exp, cnt, 3)) ok = false;
        else if (!test_factors(pf, exp, cnt, 4)) ok = false;
        else if (test_factors(pf, exp, cnt, 5)) ok = false; /* 5 must NOT be residue */

        if (ok) {
            append_cache((seq_t)cand);
        }
        ++cand;
    }
    return cache[n - 1];
}
