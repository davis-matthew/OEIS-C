/*
 * A142142  Primes congruent to 33 mod 37.
 * https://oeis.org/A142142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime congruent to 33 mod 37 (computed by brute
 *            force search)
 * Range:     u8 n <= 2, u16 n <= 169, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;
    static idx_t next_k = 0;   /* candidate index for number 33 + 37*k */

    if (n == 0) return 0;

    /* Return previously computed value if available */
    if (cache_size >= n) return cache[n - 1];

    /* Generate further terms until we reach n */
    while (cache_size < n)
    {
        seq_t cand = 33 + (seq_t)37 * (seq_t)next_k;
        if (is_prime((idx_t)cand))
        {
            if (cache_size >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                while (new_cap <= cache_size) new_cap *= 2;
                seq_t *new_arr = seq_alloc_new_array(new_cap);
                if (cache)
                {
                    memcpy(new_arr, cache, sizeof(seq_t) * cache_cap);
                    seq_free_array(cache);
                }
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
        next_k++;
    }
    return cache[n - 1];
}
