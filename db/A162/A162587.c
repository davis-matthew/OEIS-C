/*
 * A162587  Prime numbers p = 8n + 7 for which k = (7 * (256^n - 16^n) + n^2)
 *          / (n * p) is an integer.
 * https://oeis.org/A162587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * a(n) = prime p = 8k+7 such that k = (7*(256^k -
 *            16^k)+k^2)/(k*p) is an integer * * The function returns the n-th
 *            such prime. * It memoizes already-computed terms and computes
 *            new ones on demand
 * Range:     u8 n <= 11, u16 n <= 195, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

static uint64_t *cache = NULL;     /* cached prime values                 */
static size_t cache_sz  = 0;       /* number of cached terms             */
static size_t cache_cap = 0;       /* allocated capacity                  */
static uint64_t next_k  = 1;       /* next n to test for prime condition */

/* --------------------------------------------------------------------- */
/* Helper: ensure cache capacity                                         */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < need) newcap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(cache, newcap * sizeof(uint64_t));
    if (tmp == NULL) abort();    /* allocation failure */
    cache = tmp;
    cache_cap = newcap;
}

/* --------------------------------------------------------------------- */
/* Helper: modular multiplication (a*b) % mod using __uint128_t          */
static inline uint64_t modmul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* --------------------------------------------------------------------- */
/* Helper: modular exponentiation (base^exp) % mod                         */
static uint64_t modpow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1U) result = modmul(result, base, mod);
        base = modmul(base, base, mod);
        exp >>= 1U;
    }
    return result;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of offset range */

    /* Generate terms until we have enough */
    while (cache_sz < (size_t)n) {
        uint64_t k  = next_k;
        uint64_t p  = 8 * k + 7;          /* p = 8k + 7      */
        if (is_prime(p)) {
            uint64_t modval = k * p;      /* modulus n*p     */
            uint64_t pow256 = modpow(256, k, modval);
            uint64_t pow16  = modpow(16,  k, modval);
            /* (256^k - 16^k) mod modval */
            uint64_t diff = (pow256 + modval - pow16) % modval;
            uint64_t term = (7 * diff) % modval;
            term = (term + ((k * k) % modval)) % modval;
            if (term == 0ULL) {           /* integer k exists */
                grow_cache(cache_sz + 1);
                cache[cache_sz++] = p;
            }
        }
        ++next_k;                         /* advance to next n */
    }
    /* Return the cached value (cast to seq_t) */
    return (seq_t)cache[n - 1];
}
