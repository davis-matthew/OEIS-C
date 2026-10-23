/*
 * A045315  Primes p such that x^8 = 2 has a solution mod p.
 * https://oeis.org/A045315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) is the n-th prime p such that 2 is an 8th power residue mod
 *            p, i.e. 2^((p-1)/gcd(8,p-1)) ≡ 1 (mod p)
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_sz  = 0;
static uint64_t next_start = 2;
static bool initialized = false;

static void reserve_cache(size_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < n) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (!initialized)
        initialized = true;

    while (cache_sz < (size_t)n)
    {
        uint64_t cand = next_start;

        /* Find the next prime candidate */
        while (true)
        {
            if (cand == 2)
                break;
            if ((cand & 1ULL) && is_prime(cand))
                break;
            ++cand;
        }

        bool ok = false;
        if (cand == 2)
        {
            ok = true; /* 2 trivial */
        }
        else
        {
            uint64_t t = cand - 1;
            uint64_t d;
            if ((t & 7ULL) == 0)       d = 8;
            else if ((t & 3ULL) == 0)  d = 4;
            else if ((t & 1ULL) == 0)  d = 2;
            else                       d = 1;
            uint64_t exp = t / d;

            seq_t r = mod_pow((seq_t)2, exp, (seq_t)cand);
            ok = (r == (seq_t)1);
        }

        if (ok)
        {
            reserve_cache(cache_sz + 1);
            cache[cache_sz++] = (seq_t)cand;
        }

        next_start = cand + 1;
    }

    return cache[n - 1];
}
