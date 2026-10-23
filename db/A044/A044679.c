/*
 * A044679  Numbers n such that string 5,2 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th number whose base‑9 representation contains the
 *            substring "52" while (n+1) does not
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_substring_52
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_substring_52.h"

/* Dynamic cache of already computed sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 0;

/* Append a new value to the cache, growing it if necessary. */
static void append_value(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Public sequence function.  The sequence is memoised in the static cache. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* sequence is undefined for index 0 */

    /* Generate values up to the requested index if not already cached. */
    while (cache_len < (size_t)n) {
        seq_t cur = last_checked + (seq_t)1;
        while (true) {
            if (has_substring_52(cur) && !has_substring_52(cur + (seq_t)1)) {
                append_value(cur);
                last_checked = cur;
                break;
            }
            ++cur;
        }
    }

    return cache[(size_t)n - 1];
}
