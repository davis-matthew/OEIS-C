/*
 * A044245  Numbers k such that string 7,2 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th integer k such that the base‑8 representation
   of k contains the substring "72" but the representation of k‑1 does not. */

#ifdef __cplusplus
#include <vector>
#else
#include <stdbool.h>
#endif
#include <stdlib.h>

/* Return true if the base‑8 representation of x contains the digit pair
   7 (more significant) followed immediately by 2 (less significant). */
static inline bool has_72(seq_t x)
{
    if (x == 0) return false;
    seq_t prev = 0;
    bool first = true;

    while (x > 0) {
        seq_t digit = x % 8;
        if (!first) {
            if (prev == 2 && digit == 7) return true;
        }
        prev = digit;
        first = false;
        x = x / 8;
    }
    return false;
}

#ifndef __cplusplus
/* C implementation: dynamic array cache */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t last_checked = 0;
#else
/* C++ implementation: std::vector cache */
static std::vector<seq_t> cache;
static seq_t last_checked = 0;
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined index, return 0 */

#ifndef __cplusplus
    while (cache_len < n) {
        last_checked += 1;                              /* advance candidate */
        if (has_72(last_checked) && !has_72(last_checked - 1)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_checked;
        }
    }
    return cache[n - 1];
#else
    while (cache.size() < n) {
        last_checked += 1;                          /* advance candidate */
        if (has_72(last_checked) && !has_72(last_checked - 1)) {
            cache.push_back(last_checked);
        }
    }
    return cache[n - 1];
#endif
}
