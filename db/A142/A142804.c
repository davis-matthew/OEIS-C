/*
 * A142804  Primes congruent to 6 mod 61.
 * https://oeis.org/A142804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 88, u32 n <= 89, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth prime p such that p ≡ 6 (mod 61) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"     /* grow_seq(seq_t **, size_t *, size_t) */
#include "../utils/is_prime_big.h" /* is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* trivial lower offset behaviour */
    if (n == 0) return 0;

    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cached_count = 0;

    /* initialise cache if needed */
    if (cache == NULL)
        grow_seq(&cache, &cache_cap, 1);

    /* generate wells p = 61*k + 6 for k = 1,2,... */
    static idx_t k = 1;              /* next k to test */
    while (cached_count < n) {
        /* evaluate candidate as 61*k + 6 */
        seq_t cand = (seq_t)(61 * k + 6);

        if (is_prime_big(cand)) {
            if (cached_count >= cache_cap)
                grow_seq(&cache, &cache_cap, cached_count + 1);
            cache[cached_count++] = cand;
        }
        ++k;
    }

    return cache[n - 1];
}
