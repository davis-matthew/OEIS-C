/*
 * A044810  Numbers n such that string 9,7 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044810
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

/* Description:  Return the nth integer n (n ≥ 1) whose decimal representation
 * contains the substring "97" while n + 1 does not.  The sequence is built
 * incrementally; subsequent calls resume searching from the previous position
 * so the function works in any call order. */

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Test whether x contains the decimal substring 97. */
static bool contains97_seq(seq_t x)
{
    while (x >= 10) {
        if ((x % 10) == 7 && ((x / 10) % 10) == 9)
            return true;
        x /= 10;
    }
    return false;
}

/* Cache for the sequence.  Separate implementations for C and C++.
 * In C mode we use malloc/realloc; in C++ mode we use std::vector. */
static idx_t cache_len = 0;
#ifdef __cplusplus
static std::vector<seq_t> cache_vec;
static seq_t next_candidate = 1;
#else
static seq_t *cache_buf = NULL;
static size_t cache_cap = 0;
static seq_t next_candidate = 1;
#endif

/* Ensure that the cache contains at least n terms. */
static void ensure_cache_size(idx_t n)
{
    if (cache_len >= n) return;

    while (cache_len < n) {
        seq_t cur = next_candidate;
        if (contains97_seq(cur) && !contains97_seq(cur + 1)) {
#ifdef __cplusplus
            cache_vec.push_back(cur);
#else
            if (cache_len == cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = realloc(cache_buf, newcap * sizeof(seq_t));
                if (!tmp) { /* memory exhausted – abort */ abort(); }
                cache_buf = tmp;
                cache_cap = newcap;
            }
            cache_buf[cache_len] = cur;
#endif
            cache_len++;
        }
        next_candidate++;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset 1 – do not care for n=0 */
    ensure_cache_size(n);
#ifdef __cplusplus
    return cache_vec[n - 1];
#else
    return cache_buf[n - 1];
#endif
}
