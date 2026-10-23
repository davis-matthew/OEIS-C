/*
 * A140843  Primes of the form 210k + 19.
 * https://oeis.org/A140843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime congruent to 19 modulo 210 (OEIS A140843)
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   grow_seq_array, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t cache_len = 0;    /* number of primes currently cached */
    static idx_t next_k = 0;       /* k to test next (next_k*210+19) */

    /* Ensure we have at least n primes cached */
    while (cache_len < n)
    {
        /* candidate = 210*next_k + 19 */
        seq_t candidate = (seq_t)next_k;
        candidate *= (seq_t)210;
        candidate += (seq_t)19;

        /* Append if prime */
        if (is_prime_big(candidate))
        {
            /* Allocate or grow array to accommodate one more element */
            grow_seq_array(&cache, &cache_cap, cache_len + 1);
            cache[cache_len] = candidate;
            ++cache_len;
        }

        /* Move to next candidate */
        ++next_k;
    }

    /* Return the nth term (indexing from 0) */
    return cache[(size_t)(n - 1)];
}
