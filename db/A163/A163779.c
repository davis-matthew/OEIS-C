/*
 * A163779  Numbers k of the form 4*j + 1 such that 2*k + 1 is a prime with
 *          primitive root 2.
 * https://oeis.org/A163779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k≡1 (mod 4) such that 2 is a primitive
 *            root modulo 2*k+1
 * Range:     u8 n <= 13, u16 n <= 1742, u32 n <= 414, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static idx_t candidate_j = 0;

static uint64_t *prime_list_u64 = NULL;
static size_t prime_list_cnt = 0;

/* Check whether 2 is a primitive root modulo prime p */
static bool is_primitive_root(seq_t p)
{
    uint64_t pm1 = ((uint64_t)p) - 1;   /* p is prime, pm1 = p-1 = 2*k */
    uint64_t phi = pm1;                  /* temporary copy for factorization */

    /* Ensure prime list is available */
    if (prime_list_u64 == NULL)
    {
        /* 10^7 is safely large for n up to about 10000 */
        build_primes_upto(10000000, (uint64_t **)&prime_list_u64, &prime_list_cnt);
    }

    for (size_t i = 0; i < prime_list_cnt; ++i)
    {
        uint64_t q = prime_list_u64[i];
        if (q * q > phi)
            break;
        if (phi % q == 0)
        {
            /* If 2^{pm1/q} ≡ 1 (mod p), then 2 is NOT a primitive root */
            if (mod_pow(2, pm1 / q, p) == (seq_t)1)
                return false;
            while (phi % q == 0)
                phi /= q;
        }
    }
    if (phi > 1)
    {
        /* Remaining prime factor */
        if (mod_pow(2, pm1 / phi, p) == (seq_t)1)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Generate terms until we have the n-th term */
    while (cache_len < n)
    {
        seq_t k = (seq_t)4 * candidate_j + 1;   /* k = 4*j + 1 */
        seq_t p = 2 * k + 1;                   /* p = 2*k + 1 */

        if (is_prime_big(p))
        {
            if (is_primitive_root(p))
            {
                if (cache_len == cache_cap)
                {
                    cache_cap = cache_cap ? cache_cap * 2 : 128;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = k;
            }
        }
        ++candidate_j;
    }

    return cache[n - 1];
}
