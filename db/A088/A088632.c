/*
 * A088632  a(1) = 2; a(n) = smallest number such that the n-th partial
 *          concatenation is a multiple of prime(n).
 * https://oeis.org/A088632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   digit_len, get_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = smallest non‑negative integer x such that 
 *   concat( a(1),a(2),...,a(n) ) is a multiple of prime(n).
 *   a(1) = 2.
 * The function uses a cache to store computed a(i) values and 
 * recomputes the required concatenation modulo the current prime. 
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k)           */
#include "../utils/digit_len.h"   /* int digit_len(seq_t x)               */
#include "../utils/pow_uint.h"    /* seq_t pow_uint(uint64_t base, idx_t exp) */

static seq_t *cache = NULL;   /* stored a(i) values                    */
static size_t cache_len = 0;  /* number of initialized entries         */
static size_t cache_cap = 0;  /* allocated capacity                    */

/* ensure cache can hold at least 'upto' entries                */
static void ensure_cache(idx_t upto)
{
    if (upto <= cache_cap) return;
    size_t new_cap = (upto > 2) ? upto * 2 : 4;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* compute the nth term of the sequence                        */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;

    /* return cached value if already computed                 */
    if (n <= (idx_t)cache_len) return cache[n-1];

    /* compute terms up to n                                   */
    for (idx_t k = (idx_t)(cache_len + 1); k <= n; ++k)
    {
        if (k == (idx_t)1)            /* a(1) = 2                    */
        {
            ensure_cache(1);
            cache[0] = (seq_t)2;
            cache_len = 1;
            continue;
        }

        /* Current prime p = prime(k)                           */
        uint32_t p32 = get_prime(k);
        seq_t   mp   = (seq_t)p32;          /* cast to seq_t             */

        /* Compute C_{k-1} mod p by iterating over previous terms */
        seq_t c_mod = 0;
        for (idx_t i = 0; i < k-1; ++i)
        {
            seq_t ai = cache[i];
            int    dlen = digit_len(ai);

            /* pow10mod = 10^dlen mod p                          */
            seq_t pow10mod = 1;
            for (int j = 0; j < dlen; ++j)
                pow10mod = (pow10mod * 10ULL) % mp;

            c_mod = (c_mod * pow10mod + ai) % mp;
        }

        /* Find smallest x with n digits (leading zeros allowed)
           such that (C_{k-1} * 10^d + x) ≡ 0 (mod p)          */
        seq_t  result = 0;
        seq_t  pow10mod_d = 10ULL % mp;   /* 10^1 mod p           */
        idx_t  d = 1;

        for (;; ++d)
        {
            /* Compute r = (-C_{k-1} * 10^d) mod p                */
            seq_t tmp = (c_mod * pow10mod_d) % mp;
            seq_t r   = (tmp == 0) ? 0 : mp - tmp;

            /* Determine digit bounds for length d              */
            seq_t low  = (d == 1) ? (seq_t)0 : pow_uint(10ULL, d-1);
            seq_t high = pow_uint(10ULL, d) - 1;

            /* Smallest x ≥ low that is ≡ r (mod p)             */
            seq_t x;
            if (r < low)
            {
                seq_t diff   = low - r;
                seq_t steps  = (diff + (mp - 1)) / mp; /* ceil(diff/p) */
                x = r + steps * mp;
            }
            else
            {
                x = r;
            }

            if (x <= high)                /* valid choice found   */
            {
                result = x;
                break;
            }

            /* Next length: multiply previous 10^d by 10 mod p   */
            pow10mod_d = (pow10mod_d * 10ULL) % mp;
        }

        /* Store the computed term */
        ensure_cache(k);
        cache[k-1] = result;
        if (cache_len < (size_t)k) cache_len = (size_t)k;
    }

    return cache[n-1];
}
