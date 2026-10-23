/*
 * A030005  (prime(n)-1)(prime(n)-3)/8.
 * https://oeis.org/A030005
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-1)*(prime(n)-3)/8 (offset 2)
 * Range:     u8 n <= 15, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* a(2) is the first term – behavior for n < 2 is irrelevant */
    if (n < 2)
        return 0;

    /* static cache of primes */
    static uint64_t *prime_cache = NULL;
    static size_t  prime_cache_len = 0;   /* number of primes stored */
    static size_t  prime_cache_cap = 0;   /* allocated capacity */
    static uint64_t last_checked = 1;     /* last number tested for primality */

    /* extend the cache until we have the nth prime */
    while (prime_cache_len < n) {
        last_checked++;                     /* next candidate number */

        if (!is_prime(last_checked))
            continue;                       /* skip non‑primes */

        /* grow capacity if needed */
        if (prime_cache_len == prime_cache_cap) {
            size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 8;
            uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
            if (!tmp) abort();
            prime_cache = tmp;
            prime_cache_cap = new_cap;
        }

        prime_cache[prime_cache_len++] = last_checked;   /* store prime */
    }

    /* prime(n) is stored at index n-1 (since a(2) uses prime(2)) */
    uint64_t p_val = prime_cache[n - 1];
    seq_t    p     = (seq_t)p_val;
    seq_t    result = ((p - (seq_t)1) * (p - (seq_t)3)) / (seq_t)8;
    return result;
}
