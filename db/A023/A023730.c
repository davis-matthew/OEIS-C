/*
 * A023730  Numbers with a single 2 in their base 5 expansion.
 * https://oeis.org/A023730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer having exactly one digit 2 in its base‑5
 *            expansion
 * Range:     u8 n <= 101, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static bool has_single_two(seq_t x)
{
    unsigned count = 0;
    while (x > 0) {
        seq_t d = x % 5;
        if (d == 2) {
            count++;
            if (count > 1)
                return false;
        }
        x = x / 5;
    }
    return count == 1;
}

seq_t a(idx_t n)
{
    /* Handle the case below the offset */
    if (n == 0)
        return 0;

    /* Static cache of previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static seq_t last_value  = 0;

    /* Extend cache until it contains at least n terms */
    while (cache_len < n) {
        /* Ensure enough capacity */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* Allocation failed – return 0 as a safe fallback */
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Find the next number with a single 2 in base‑5 */
        seq_t candidate = last_value + 1;
        while (!has_single_two(candidate))
            candidate++;

        cache[cache_len++] = candidate;
        last_value = candidate;
    }

    return cache[n - 1];
}
