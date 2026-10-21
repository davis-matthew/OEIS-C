/*
 * A093665  Numbers k that divide 2^(k^3) + 1.
 * https://oeis.org/A093665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that k | 2^(k^3) + 1 (OEIS
 *            sequence A093665). Computation is performed by testing
 *            successive odd k and verifying the congruence using modular
 *            exponentiation and Euler's theorem. The result is memoised so
 *            that a(n) can be requested in any order
 * Range:     u8 n <= 8, u16 n <= 32, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mod_pow.h"

/* The cache holds already computed terms of the sequence. */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static uint64_t next_k = 1;   /* next odd integer to be tested */

/* ------------------------------------------------------------ */
/*  Prime sieve – up to a fixed large bound (2'000'000).      */
/* ------------------------------------------------------------ */
static uint64_t *primes_arr = NULL;
static size_t primes_len = 0;

static void init_primes(void)
{
    if (primes_arr != NULL) return;           /* already initialised */
    const uint64_t LIM = 2000000ULL;          /* enough for n <= 10000 */
    primes_arr = (uint64_t*)malloc((LIM + 1) * sizeof(uint64_t));
    size_t *is_composite = (size_t*)calloc(LIM + 1, sizeof(size_t));
    primes_len = 0;
    for (uint64_t i = 2; i <= LIM; ++i) {
        if (!is_composite[i]) {
            if (primes_len == LIM + 1) break;
            primes_arr[primes_len++] = i;
            if (i * i <= LIM) {
                for (uint64_t j = i * i; j <= LIM; j += i) {
                    is_composite[j] = 1;
                }
            }
        }
    }
    free(is_composite);
}

/* ------------------------------------------------------------ */
/*  Euler's totient function for uint64_t, using the prime list */
/* ------------------------------------------------------------ */
static uint64_t phi_uint64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t result = n;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_len && primes_arr[i] * primes_arr[i] <= tmp; ++i) {
        uint64_t p = primes_arr[i];
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            result -= result / p;
        }
    }
    if (tmp > 1) result -= result / tmp;
    return result;
}

/* ------------------------------------------------------------ */
/*  Test whether k satisfies k | 2^(k^3)+1.                       */
/* ------------------------------------------------------------ */
static bool satisfies(uint64_t k)
{
    if (k == 1) return true;   /* 1 divides every integer */
    uint64_t phi_k = phi_uint64(k);
    /* compute e = k^3 mod phi_k */
    uint64_t km = k % phi_k;
    uint64_t k2 = (__uint128_t)km * km % phi_k;
    uint64_t e = (__uint128_t)k2 * km % phi_k;

    seq_t pow_val = mod_pow((seq_t)2, e, (seq_t)k);
    seq_t sum = pow_val + (seq_t)1;
    return (sum % (seq_t)k == 0);
}

/* ------------------------------------------------------------ */
/* Grow the cache to hold at least 'need' elements.            */
/* ------------------------------------------------------------ */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ------------------------------------------------------------ */
/* Main sequence function                                     */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined offset; harmless */

    /* initialise static structures on first call */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache_cap = 1;
        cache_len = 0;
    }
    if (primes_arr == NULL) init_primes();

    /* ensure we have at least 'n' terms cached */
    while (cache_len < (size_t)n) {
        /* test current candidate next_k (always odd) */
        while (!satisfies(next_k)) {
            next_k += 2;
        }
        grow_cache(cache_len + 1);
        cache[cache_len++] = (seq_t)next_k;
        next_k += 2;   /* move to next odd number */
    }
    return cache[(size_t)n - 1];
}
