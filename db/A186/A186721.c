/*
 * A186721  Numbers n such that n, 4n + 1 and 6n + 1 are all prime.
 * https://oeis.org/A186721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers n such that n, 4n + 1 and 6n + 1 are all prime
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Cache previous results for fast re‑use. */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t last_checked = 0;

    if (n == 0) return 0;          /* no meaningful term below offset 1 */

    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* Expand cache if needed. */
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Search for new qualifying numbers until we have at least n terms. */
    for (idx_t k = last_checked + 1;; ++k) {
        /* Test the three primes. */
        if (is_prime(k) &&
            is_prime((idx_t)(4 * k + 1)) &&
            is_prime((idx_t)(6 * k + 1))) {

            cache[cache_size++] = (seq_t)k;

            if (cache_size == n) {
                last_checked = k;
                return cache[n - 1];
            }
        }
        last_checked = k;
    }
}
