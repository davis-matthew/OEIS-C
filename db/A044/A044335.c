/*
 * A044335  Numbers k such that the string 0,3 occurs in the base 10
 *          representation of k but not of k-1.
 * https://oeis.org/A044335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer k such that decimal representation
 *            of k * contains the substring "03" but (k-1) does not
 * Range:     u8 n <= 2, u16 n <= 720, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* static cache for memoised terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 0;

/* check if a decimal number contains the substring "03" */
static inline bool has_03(uint64_t x)
{
    if (x < 3) return false;
    while (x >= 10) {
        uint64_t d1 = x % 10;   /* lower digit  */
        x /= 10;
        uint64_t d0 = x % 10;   /* higher digit */
        if (d1 == 3 && d0 == 0) return true;
    }
    return false;
}

/* add a new term to the cache, resizing as necessary */
static void add_term(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* OEIS sequence A044335 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset starts at 1 */
    if (n <= cache_len) return cache[n-1];

    while (cache_len < n) {
        last_checked++;       /* next integer to test */
        if (has_03(last_checked) && !has_03(last_checked-1))
            add_term((seq_t)last_checked);
    }
    return cache[n-1];
}
