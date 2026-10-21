/*
 * A215434  Primes of form 2^k + k^2 - 1.
 * https://oeis.org/A215434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 2^k + k^2 - 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big, pow2, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static idx_t last_k = 0;

    /* Initialize cache once */
    if (!cache) {
        cache_cap = 64;
        cache = seq_alloc_new_array(cache_cap);
        cache_size = 0;
        last_k = 0;
    }

    /* Generate primes until we have enough */
    while (cache_size < n) {
        last_k++;
        seq_t val = pow2(last_k);
        seq_t k_sq = (seq_t)last_k * (seq_t)last_k;
        val += k_sq;
        val -= (seq_t)1;
        if (is_prime_big(val)) {
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_size++] = val;
        }
    }

    return cache[n - 1];
}
