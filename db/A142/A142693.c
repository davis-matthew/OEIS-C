/*
 * A142693  Primes congruent to 46 mod 57.
 * https://oeis.org/A142693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 182, u32 n <= 118, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 46 mod 57 */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_k = 0;   /* last tested candidate index */

    if (cache == NULL) {
        cache_cap = 16;
        cache = alloc_seq_array(cache_cap);
        cache_len = 0;
        last_k = 0;
    }

    while (cache_len < n) {
        idx_t val = 46 + (idx_t)57 * last_k;
        seq_t candidate = (seq_t)val;
        if (is_prime_big(candidate)) {
            if (cache_len >= cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = candidate;
        }
        ++last_k;
    }

    return cache[n - 1];
}
