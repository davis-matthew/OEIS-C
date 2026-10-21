/*
 * A053664  Smallest number m such that m == i (mod prime(i)) for all 1<=i<=n.
 * https://oeis.org/A053664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   get_prime, mod_pow64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A053664: a(n) = minimal m such that m ≡ i (mod p_i) for all 1 ≤ i ≤ n,
   where p_i is the i-th prime.
   Computed by incremental Chinese Remainder Theorem. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow64.h"

seq_t a(idx_t n)
{
    /* static state to allow out‑of‑order queries */
    static seq_t *cache = NULL;    /* cache of a(i) for i = 1…cache_len */
    static size_t cache_len = 0;   /* number of values stored */
    static size_t cache_cap = 0;   /* capacity of cache array */
    static seq_t cur_x = 0;        /* current solution for a(cache_len)   */
    static seq_t cur_M = 1;        /* product of the processed primes    */
    static idx_t cur_n = 0;        /* last processed index (== cache_len)*/

    /* extend cache up to n */
    while (cache_len < n)
    {
        /* index of the next term (1‑based) */
        idx_t i = cache_len + 1;

        /* next prime p_i */
        uint64_t prime = (uint64_t)get_prime(i);

        /* current modulus product modulo p_i */
        seq_t M_mod_p_seq = cur_M % (seq_t)prime;
        uint64_t M_mod_p = (uint64_t)M_mod_p_seq;

        /* i modulo prime (required residue) */
        uint64_t residue = i % prime;

        /* current solution modulo prime */
        uint64_t cur_mod = (uint64_t)(cur_x % (seq_t)prime);

        /* difference needed to satisfy new congruence */
        uint64_t diff = (residue + prime - cur_mod) % prime;

        /* modular inverse of M_mod_p modulo prime */
        uint64_t inv = mod_pow64(M_mod_p, prime - 2, prime);

        /* step multiplier */
        uint64_t t = (diff * inv) % prime;

        /* update solution: x = x + t * M */
        seq_t prod = cur_M * (seq_t)t;
        cur_x = cur_x + prod;

        /* update modulus product: M = M * prime */
        cur_M = cur_M * (seq_t)prime;

        /* grow cache if needed */
        if (cache_len >= cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* store computed term */
        cache[cache_len++] = cur_x;
    }

    return cache[n - 1];
}
