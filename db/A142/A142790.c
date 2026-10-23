/*
 * A142790  Primes congruent to 23 mod 60.
 * https://oeis.org/A142790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime that is congruent to 23 (mod 60)
 * Range:     u8 n <= 2, u16 n <= 414, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/ensure_capacity.h"
#include "../utils/is_prime.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_candidate = 23;

/* Ensure that the cache holds at least one term (the first prime 23). */
static void init_cache(void)
{
    if (cache == NULL)
    {
        cache_cap = 16;
        cache = alloc_seq_array(cache_cap);
        cache[0] = 23;
        cache_len = 1;
        last_candidate = 23;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* not defined for n < 1, but safe guard */

    init_cache();

    while (cache_len < n)
    {
        idx_t candidate = last_candidate + 60;          /* next number ≡ 23 (mod 60) */
        for (;;)
        {
            if (is_prime(candidate))
            {
                size_t need = cache_len + 1;
                ensure_capacity(need, &cache_cap, (void **)&cache, sizeof(seq_t));
                cache[cache_len++] = (seq_t)candidate;
                last_candidate = candidate;
                break;
            }
            candidate += 60;        /* skip to next ≡ 23 mod 60 */
        }
    }

    return cache[n - 1];
}
