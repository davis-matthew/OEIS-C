/*
 * A209309  Numbers whose sum of triangular divisors is also triangular and
 *          greater than 1.
 * https://oeis.org/A209309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 54, u32 n <= 8055, u64 n <= 8055,
 *            u128 n <= 8055, big any n
 * Validated: standard, pass, 70 terms through n = 7926 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose sum of triangular divisors is triangular > 1 (OEIS A209309) */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Check whether a seq_t number is triangular: 8*x+1 is a perfect square. */
static bool is_triangular_seq(seq_t x)
{
    if (x == 0) return false;
    seq_t y = 8 * x + 1;
    seq_t r = floor_sqrt(y);
    return r * r == y;
}

/* Test whether an integer v has the required property. */
static bool has_property(idx_t v)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= v; ++d)
    {
        if (v % d == 0)
        {
            if (is_triangular_seq((seq_t)d))
                sum += (seq_t)d;
            idx_t other = v / d;
            if (other != d && is_triangular_seq((seq_t)other))
                sum += (seq_t)other;
        }
    }
    return (sum > (seq_t)1) && is_triangular_seq(sum);
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_test = 1;
    static idx_t found = 0;

    while (found < n)
    {
        if (has_property(next_test))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap == 0 ? 32 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0;  /* allocation failure sentinel */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_test;
            ++found;
        }
        ++next_test;
    }

    return cache[n - 1];
}
