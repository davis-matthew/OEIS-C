/*
 * A171845  Numbers n >= 0 such that n-1 and n+1 are both primes or both
 *          nonprimes, but excluding primes.
 * https://oeis.org/A171845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th integer n ≥ 0 such that n is not prime and
 *            isPrime(n‑1) == isPrime(n+1) (excluding primes)
 * Range:     u8 n <= 130, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 0;

seq_t a(idx_t n)
{
    /* offset 1; n ≥ 1 for valid calls */
    while (cache_len < (size_t)n)
    {
        seq_t cand = (seq_t)next_candidate;
        bool left_prime  = (next_candidate > 0)
                          ? is_prime((idx_t)(next_candidate - 1))
                          : false;
        bool right_prime = is_prime((idx_t)(next_candidate + 1));
        bool not_prime   = !is_prime((idx_t)next_candidate);

        if (left_prime == right_prime && not_prime)
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
