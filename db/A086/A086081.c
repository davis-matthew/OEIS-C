/*
 * A086081  Numbers m such that m and its 2's complement are both primes. In
 *          other words, m and 2^k - m (where k is the smallest power of 2
 *          such that 2^k > m) are primes.
 * https://oeis.org/A086081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1039, u32 n <= 2731, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime_big, next_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A086081: primes m such that 2^k−m is also prime,
   where k = floor(log₂(m)) + 1.  The function generates terms on demand
   and caches them. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"
#include "../utils/grow_seq.h"
#include "../utils/pow2.h"

/* Cache for sequence values */
static seq_t *cache      = NULL;    /* stored terms */
static idx_t  cache_len  = 0;       /* number of terms stored   */
static idx_t  cache_cap  = 0;       /* capacity of cache        */
static uint64_t last_prime = 1;     /* last prime examined      */

/* Helper: bit length of an unsigned 64‑bit integer */
static unsigned bitlen_u64(uint64_t v)
{
    return v ? 64 - __builtin_clzll(v) : 0;
}

/* Return the nth term of A086081 (1‑based index). */
seq_t a(idx_t n)
{
    /* Fill the cache until it contains at least n terms */
    while (cache_len < n)
    {
        /* Get the next prime after the last one examined */
        uint64_t p = next_prime(last_prime);
        last_prime = p;

        /* Stop if the prime no longer fits into seq_t */
        if (bitlen_u64(p) > sizeof(seq_t) * 8)
            break;

        unsigned k = bitlen_u64(p);          /* k = floor(log2(p)) + 1 */
        seq_t sp = (seq_t)p;                  /* current prime in seq_t */

        /* Compute the complement 2^k – p */
        seq_t comp;
        if (sizeof(seq_t) * 8 <= 64)
        {
            /* Use 128‑bit arithmetic to avoid overflow and signedness
               issues when k == 64. */
            __uint128_t two_power = ((__uint128_t)1) << k;
            comp = (seq_t)(two_power - (__uint128_t)p);
        }
        else
        {
            seq_t two_power = pow2((idx_t)k);
            comp = two_power - sp;
        }

        /* If both numbers are prime, store the prime in the cache */
        if (is_prime_big(comp))
        {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = sp;
        }
    }

    /* Return the requested term if it was computed, otherwise 0. */
    return (n > 0 && n <= cache_len) ? cache[n - 1] : (seq_t)0;
}
