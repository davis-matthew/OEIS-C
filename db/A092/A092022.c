/*
 * A092022  Numbers k such that 16k + 3 is prime.
 * https://oeis.org/A092022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p_n - 3)/16, where p_n is the nth prime congruent to 3
 *            modulo 16
 * Range:     u8 n <= 9, u16 n <= 1071, u32 n <= 483, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;         /* Cached a(n) values */
static size_t cache_cap = 0;        /* Current allocation size */
static size_t cache_size = 0;       /* Number of values currently stored */
static idx_t next_k = 0;            /* Next k to test (16*k+3) */
static size_t found = 0;            /* Number of valid a(n) found so far */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* Undefined for n < 1, but return 0 */

    /* Return cached value if already computed */
    if (n <= cache_size)
        return cache[n - 1];

    /* Extend the cache until we have at least n terms */
    while (found < n) {
        seq_t p = (seq_t)next_k * (seq_t)16 + (seq_t)3;
        if (is_prime_big(p)) {
            seq_t val = (p - (seq_t)3) / (seq_t)16;

            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) /* allocation failed – abort */
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }

            cache[cache_size++] = val;
            found++;
        }
        next_k++;
    }

    return cache[n - 1];
}
