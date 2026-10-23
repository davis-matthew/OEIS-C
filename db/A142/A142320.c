/*
 * A142320  Primes congruent to 16 mod 45.
 * https://oeis.org/A142320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime congruent to 16 modulo 45
 * Range:     u8 n <= 3, u16 n <= 275, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* cache of found primes */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* last number that was tested; starts just before the first
       required prime (61). */
    static uint64_t last_checked = 59;

    if (n == 0)
        return (seq_t)0;                 /* undefined for n < 1 */

    /* grow the cache until we have the nth term */
    while (cache_len < n) {
        /* advance to the next odd number that is 16 (mod 45) */
        do {
            last_checked += 2;           /* keep odd */
        } while (last_checked % 45 != 16);

        /* reject overflow; if this happens we cannot continue */
        if (last_checked > (uint64_t)-2) {
            return (seq_t)0;
        }

        if (is_prime(last_checked)) {
            /* grow the cache if necessary */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
