/*
 * A142604  Primes congruent to 4 mod 55.
 * https://oeis.org/A142604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 165, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The nth prime ≡ 4 (mod 55). This implementation searches for primes
   incrementally and caches the results for repeated calls. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Offset 1: return 0 for n == 0 (undefined). */
    if (n == 0)
        return (seq_t)0;

    /* static cache of found primes of the form 55*k + 4   */
    static seq_t *prime_cache = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_count = 0;
    static idx_t  current_k    = 1;   /* k such that 55*k+4 is the candidate */

    while (cache_count < (size_t)n)
    {
        idx_t candidate = (idx_t)55 * current_k + 4;
        if (is_prime(candidate))
        {
            if (cache_count == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);      /* allocation failure */
                prime_cache = tmp;
                cache_cap = new_cap;
            }
            prime_cache[cache_count++] = (seq_t)candidate;
        }
        ++current_k;
    }

    return prime_cache[n-1];
}
