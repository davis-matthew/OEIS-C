/*
 * A044750  Numbers n such that string 3,7 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) by searching sequentially for numbers whose decimal
   representation contains the substring "37" but n+1 does not. */

#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;
static idx_t next_scan = 1;
#else
#include <stdlib.h>
static seq_t *cache = NULL;
static size_t sz = 0, cap = 0;
static idx_t next_scan = 1;
#endif

/* Check whether a decimal representation of x contains the consecutive
   digits 3 followed by 7. */
static bool contains37(seq_t x)
{
    const seq_t ten = 10;
    seq_t prev = ten;               /* sentinel larger than any digit */
    while (x > 0) {
        seq_t d = x % ten;
        if (prev == 7 && d == 3) return true;
        prev = d;
        x /= ten;
    }
    return false;
}

/* Grow the cache in pure C. */
#ifndef __cplusplus
static void grow_cache(void)
{
    if (sz == cap) {
        size_t new_cap = (cap == 0) ? 128 : cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cap = new_cap;
    }
}
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for 0 */

#ifndef __cplusplus
    while (sz < n) {
        seq_t x = next_scan;
        seq_t y = x + 1;
        if (contains37(x) && !contains37(y)) {
            grow_cache();
            cache[sz++] = x;
        }
        ++next_scan;
    }
    return cache[n - 1];
#else
    while (cache.size() < n) {
        seq_t x = next_scan;
        seq_t y = x + 1;
        if (contains37(x) && !contains37(y)) {
            cache.push_back(x);
        }
        ++next_scan;
    }
    return cache[n - 1];
#endif
}
