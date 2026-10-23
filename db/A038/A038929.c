/*
 * A038929  Primes p such that 51 is a square mod p.
 * https://oeis.org/A038929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038929: primes p such that 51 is a quadratic residue mod p. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/is_prime.h"  /* bool is_prime(idx_t x); */

/* find the n-th prime p with (51|p)=1 or 51≡0 (mod p) */
seq_t a(idx_t n)
{
    /* static cache of found primes satisfying the condition */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* next candidate odd number to test; start from 3 */
    static uint64_t candidate = 3;

    /* extend cache until we have at least n elements */
    while (cache_len < n)
    {
        /* test if candidate is prime */
        if (is_prime(candidate))
        {
            /* if candidate divides 51, 51≡0≡0² mod candidate – accept */
            if (51 % candidate == 0)
            {
                /* store candidate */
                if (cache_len == cache_cap)
                {
                    cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)candidate;
            }
            else
            {
                /* compute Legendre (51|candidate) via Euler's criterion */
                seq_t residue = mod_pow((seq_t)51,
                                        ((candidate - 1) >> 1), /* (p-1)/2 */
                                        (seq_t)candidate);
                if (residue == (seq_t)1)
                {
                    /* store candidate */
                    if (cache_len == cache_cap)
                    {
                        cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[cache_len++] = (seq_t)candidate;
                }
            }
        }
        /* move to next odd number */
        candidate += 2;
    }

    /* retrieve desired term (indexing starts at 1) */
    return cache[n - 1];
}
