/*
 * A044429  Numbers n such that string 9,7 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sequence of integers n such that the decimal representation
   of n contains the substring "97" but the representation of n-1 does not.
   The n-th term is returned by a(n). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Check whether a decimal representation of x contains "97" */
static inline bool has_97(seq_t x)
{
    if (x < 10) return false;
    seq_t y = x;
    while (y >= 10) {
        seq_t last  = y % 10;
        seq_t next  = (y / 10) % 10;
        if (next == 9 && last == 7) return true;
        y /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t curnum = 0;   /* last integer that has been examined */

#ifndef __cplusplus
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    while (cache_len < n) {
        ++curnum;
        if (has_97(curnum) && !has_97(curnum - 1)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();    /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = curnum;
        }
    }
    return cache[n - 1];
#else
    static std::vector<seq_t> cache;
    while (cache.size() < n) {
        ++curnum;
        if (has_97(curnum) && !has_97(curnum - 1)) {
            cache.push_back(curnum);
        }
    }
    return cache[n - 1];
#endif
}
