/*
 * A203468  Numbers that have a unique triangular proper divisor greater than
 *          1.
 * https://oeis.org/A203468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer that has exactly one proper
 *            divisor greater than 1 which is a triangular number
 * Range:     u8 n <= 3, u16 n <= 756, u32 n <= 1942, u64 n <= 1942,
 *            u128 n <= 1942, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Helper to test if a number is triangular */
static bool is_triangular(seq_t d)
{
    seq_t val = 1 + 8 * d;
    seq_t s   = floor_sqrt(val);
    return s * s == val && ((s - 1) % 2 == 0);
}

/* Simple dynamic array for the cached sequence values */
static seq_t *seq_cache      = NULL;
static size_t cache_len      = 0;   /* number of elements stored */
static size_t cache_cap      = 0;   /* allocated capacity */
static idx_t  last_checked   = 0;   /* last integer examined */

/* Grow the cache array to at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1); /* out of memory */
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* True iff integer m has exactly one proper divisor >1 that is triangular */
static bool has_unique_triangular_divisor(idx_t m)
{
    idx_t cnt = 0;
    for (idx_t d = 2; d <= m / d; ++d) {
        if (m % d == 0) {
            seq_t d_val = d;
            if (is_triangular(d_val)) {
                if (++cnt > 1)
                    return false;
            }
            idx_t e = m / d;
            if (e != d && e != m && e > 1) {
                seq_t e_val = e;
                if (is_triangular(e_val)) {
                    if (++cnt > 1)
                        return false;
                }
            }
        }
    }
    return cnt == 1;
}

/* Return the n-th term of the sequence (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 0 */

    /* Return cached value if already known */
    if (n <= cache_len)
        return seq_cache[n - 1];

    /* Ensure the cache has enough capacity for new entries */
    if (cache_len == 0)
        grow_cache(16);

    /* Search forward until we have found the n-th term */
    idx_t m = last_checked + 1;
    while (cache_len < n) {
        if (has_unique_triangular_divisor(m)) {
            /* Append m to the cache */
            if (cache_len == cache_cap)
                grow_cache(cache_cap * 2);
            seq_cache[cache_len++] = m;
        }
        ++m;
    }
    last_checked = m - 1;
    return seq_cache[n - 1];
}
