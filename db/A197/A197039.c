/*
 * A197039  Numbers such that sum of the cube of decimal digits is a perfect
 *          square.
 * https://oeis.org/A197039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A197039: numbers whose sum of the cubes of decimal digits is a perfect square.
   Computation performed by linear search with a memoised cache. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool is_square(uint32_t x)
{
    uint32_t r = 0;
    while ((r + 1u) * (r + 1u) <= x)
        ++r;
    return r * r == x;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of found terms */
    static size_t cache_cap = 0;  /* capacity of cache */
    static idx_t found = 0;       /* number of terms stored */
    static idx_t candidate = 1;   /* next integer to test */

    /* Pre‑computed cubes of digits 0–9. */
    static const unsigned int cube[10] = {
        0u, 1u, 8u, 27u, 64u, 125u,
        216u, 343u, 512u, 729u
    };

    /* Generate terms until we have at least n of them. */
    while (found < n) {
        idx_t tmp = candidate;
        uint32_t sum = 0u;

        /* Sum of cubes of decimal digits of candidate. */
        while (tmp > 0u) {
            unsigned int d = (unsigned int)(tmp % 10u);
            sum += cube[d];
            tmp /= 10u;
        }

        if (is_square(sum)) {
            if (found >= cache_cap) {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
                seq_t *tmp_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp_arr == NULL)
                    abort();          /* allocation failure */
                cache       = tmp_arr;
                cache_cap   = new_cap;
            }
            cache[found] = (seq_t)candidate;
            ++found;
        }
        ++candidate;
    }

    return cache[n - 1];
}
