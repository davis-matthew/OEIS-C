/*
 * A223881  Denominators in the expression m!/(prime(m-1)+1) for m > 1 such
 *          that this expression is not an integer.
 * https://oeis.org/A223881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************
 *  Sequence A223881
 *
 *  a(n) = denominator of (m!)/(prime(m-1)+1)
 *  for the nth m > 1 that makes the fraction non‑integral.
 *
 *  The function returns the exact denominator (in the lowest terms),
 *  using the type seq_t supplied by the build system.
 *********************************************/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------- helpers from the common utils ------- */
#include "../utils/gcd.h"          /* not strictly needed, kept for consistency */

/* ------- fast integer exponentiation for seq_t ------- */
static seq_t seq_pow_small(seq_t base, unsigned int exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1U)
            result = result * base;
        base = base * base;
        exp >>= 1U;
    }
    return result;
}

/* ------- exponent of prime p in m!  (Legendre's formula) ------- */
static unsigned int exp_in_factorial(idx_t m, uint32_t p)
{
    unsigned int e = 0;
    while (m) {
        m /= p;
        e += (unsigned int)m;
    }
    return e;
}

/* ------- sieve for primes up to a fixed limit ------- */
#define SIEVE_LIMIT 200000ULL        /* sufficient for prime(9999)+1 (~104730) */

static bool sieve_initialized = false;
static uint32_t *prime_list = NULL;   /* 0‑based: prime_list[0] = 2          */
static size_t prime_count     = 0;

static void init_sieve(void)
{
    if (sieve_initialized) return;
    sieve_initialized = true;

    bool *is_comp = (bool*)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_comp) abort();

    size_t cap = 20000;               /* initial capacity for prime list     */
    prime_list = (uint32_t*)malloc(cap * sizeof(uint32_t));
    if (!prime_list) abort();

    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= SIEVE_LIMIT; j += i)
                is_comp[j] = true;
        }
    }
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_count == cap) {
                cap <<= 1;
                prime_list = (uint32_t*)realloc(prime_list, cap * sizeof(uint32_t));
                if (!prime_list) abort();
            }
            prime_list[prime_count++] = i;
        }
    }
    free(is_comp);
}

/* ------- cache for computed denominators ------- */
static seq_t *seq_cache   = NULL;   /* array of a(i) values               */
static size_t seq_cache_cap = 0;    /* its capacity                       */
static size_t seq_cache_len = 0;    /* actual number of stored elements   */
static idx_t   next_m       = 2;    /* next m to process                 */

/* ---- ensure the cache can hold at least new_len elements ---- */
static void ensure_seq_cache_capacity(size_t new_len)
{
    if (new_len <= seq_cache_cap) return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap << 1 : 16;
    if (new_cap < new_len) new_cap = new_len;
    seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!seq_cache) abort();
    seq_cache_cap = new_cap;
}

/* ---- extend the cache until it contains at least n values ---- */
static void extend_cache_to(size_t n)
{
    while (seq_cache_len < n) {
        if (next_m < 2) {            /* safety guard, should never happen   */
            abort();
        }

        /* ensure we have enough primes for prime(next_m-1)          */
        if (next_m - 2ULL >= (idx_t)prime_count) {
            /* In practice this won't happen with SIEVE_LIMIT 200k. */
            abort();
        }
        uint32_t p = prime_list[next_m - 2ULL];     /* prime(m-1)           */
        seq_t d = (seq_t)p + (seq_t)1;              /* denominator before cancelling */

        /* factor d using the pre‑computed prime list            */
        idx_t temp = (idx_t)d;
        uint32_t factors[32];
        unsigned int deg[32];
        size_t nf = 0;

        for (size_t i = 0; (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= (uint64_t)temp; ++i) {
            uint32_t fp = prime_list[i];
            if (temp % fp == 0) {
                unsigned int e = 0;
                while (temp % fp == 0) {
                    temp /= fp;
                    ++e;
                }
                factors[nf] = fp;
                deg[nf] = e;
                ++nf;
            }
        }
        if (temp > 1) { /* remaining prime factor                 */
            if (nf == sizeof(factors)/sizeof(factors[0])) abort();
            factors[nf] = (uint32_t)temp;
            deg[nf] = 1;
            ++nf;
        }

        /* compute g_d = product of fp^min(deg, exp_in_factorial(next_m,fp)) */
        seq_t g_d = (seq_t)1;
        for (size_t i = 0; i < nf; ++i) {
            unsigned int e_m = exp_in_factorial(next_m, factors[i]);
            unsigned int e_g = deg[i] < e_m ? deg[i] : e_m;
            if (e_g)
                g_d = g_d * seq_pow_small((seq_t)factors[i], e_g);
        }

        seq_t denom = d / g_d;     /* reduce the fraction                */

        if (denom > (seq_t)1) {
            ensure_seq_cache_capacity(seq_cache_len + 1);
            seq_cache[seq_cache_len++] = denom;
        }

        ++next_m;                  /* move to next m                     */
    }
}

/* ------- public interface required by the framework ------- */
seq_t a(idx_t n)
{
    if (n < 1) abort();       /* offset 1: a(1) is the first term */

    init_sieve();
    if (seq_cache_len < (size_t)n)
        extend_cache_to((size_t)n);

    return seq_cache[n - 1];
}
