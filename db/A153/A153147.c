/*
 * A153147  a(n) = A007916(n)^3.
 * https://oeis.org/A153147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007916(n)^3 where A007916(n) is the n‑th positive
 *            integer that is not a perfect power
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1574, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

/* Check whether x is a perfect power (x = a^b with a ≥ 2, b ≥ 2). */
static bool is_perfect_power(idx_t x)
{
    if (x <= 1) return false;
    for (idx_t base = 2; base * base <= x; ++base) {
        seq_t val = base * base;
        while (val < x) {
            if (val > x / base) break;   /* prevent overflow */
            val *= base;
        }
        if (val == x) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for index 0, safe fallback */

    /* Persistent cache of computed terms. */
    static seq_t *cache      = NULL;   /* a(k) values */
    static size_t cache_len  = 0;       /* number of cached terms */
    static size_t cache_cap  = 0;       /* allocated capacity   */
    static idx_t  current    = 2;       /* next integer to test */

    while (cache_len < n) {
        /* Find the next integer that is not a perfect power. */
        while (current <= 1 || is_perfect_power(current)) ++current;

        /* Compute its cube. */
        seq_t val = current;
        val = val * val * val;

        /* Grow the cache array if needed. */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = val;
        ++current;
    }

    return cache[n - 1];
}
