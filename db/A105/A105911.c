/*
 * A105911  Primes for which -39 is a primitive root.
 * https://oeis.org/A105911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, modpow_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of primes p (including 2) such that
   -39 is a primitive root modulo p.  Equivalently
   g = (-39 mod p) must have order p-1.  For each prime p>3,13
   we test that g^{(p-1)/q} ≠ 1 for every prime divisor q of p-1.  */

/* Note: uses the utilities is_prime(uint64_t) and
   modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/modpow_uint64.h"

static seq_t *cache = NULL;           /* stored terms */
static idx_t cache_size = 0;          /* number of terms stored */
static idx_t cache_capacity = 0;      /* allocated size */
static uint64_t next_candidate = 2;   /* next integer to test for primality */

/* Ensure the cache has enough room for 'needed' elements. */
static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= cache_capacity)
        return;
    idx_t new_cap = cache_capacity == 0 ? 16 : cache_capacity << 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();               /* out of memory */
    cache = tmp;
    cache_capacity = new_cap;
}

/* Returns the nth term of A105911 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset is 1, so this is never used */

    /* Generate terms up to n if necessary. */
    if (cache_size < n)
    {
        ensure_cache_capacity(n);
        while (cache_size < n)
        {
            uint64_t p = next_candidate;
            next_candidate++;

            /* skip even numbers >2 to save tests */
            if (p > 2 && (p & 1ULL) == 0)
                continue;

            if (!is_prime(p))
                continue;

            /* p = 2 is accepted; 3 and 13 are discarded because -39 ≡ 0 mod p */
            if (p == 3 || p == 13)
                continue;

            /* Compute g = (-39 mod p) */
            uint64_t r = 39 % p;
            uint64_t g = (p - r) % p;   /* g in [0, p-1] */
            if (g == 0)                 /* p divides 39 */
                continue;

            /* If p is odd prime, test that g is a primitive root. */
            uint64_t m = p - 1;
            uint64_t temp = m;
            int is_primitive = 1;

            /* Factor m and test each distinct prime factor q */
            for (uint64_t d = 2; d * d <= temp; ++d)
            {
                if (temp % d == 0)
                {
                    /* d is a prime factor of m */
                    if (modpow_uint64(g, m / d, p) == 1)
                    {
                        is_primitive = 0;
                        break;
                    }
                    while (temp % d == 0)
                        temp /= d;
                }
            }
            if (is_primitive && temp > 1)
            {
                /* temp itself is a prime factor now */
                if (modpow_uint64(g, m / temp, p) == 1)
                    is_primitive = 0;
            }

            if (is_primitive)
            {
                cache[cache_size++] = (seq_t)p;
            }
        }
    }

    return cache[n - 1];
}
