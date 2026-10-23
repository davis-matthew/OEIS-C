/*
 * A044211  Numbers n such that string 3,0 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer such that the string "30" occurs in
 *            its base‑8 representation but not in that of n-1
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_30_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_30_base8.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_capacity = 0;
static idx_t last_checked = 0;

static void cache_append(seq_t val)
{
    if (cache_size == cache_capacity) {
        size_t new_cap = cache_capacity == 0 ? 16 : cache_capacity * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_capacity = new_cap;
    }
    cache[cache_size++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* index 0 is undefined for this sequence */
    /* Build cache up to the requested index */
    while (cache_size < (size_t)n) {
        idx_t m = last_checked + 1;
        ++last_checked;
        if (has_30_base8(m) && !has_30_base8(m - 1)) {
            cache_append((seq_t)m);
        }
    }
    return cache[n - 1];
}
