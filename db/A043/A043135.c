/*
 * A043135  Numbers k such that 0 and 5 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = the n‑th integer k such that 
   the base‑7 representation of k contains an adjacent pair
   0–5 or 5–0 while the representation of k–1 does not.
   The result is cached so that calls with arbitrary n are fast. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;        /* Cache for C mode */
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t last_checked = 0;     /* Largest integer already examined */

/* Base‑7 adjacency check. */
static bool has_adjacent_0_5(seq_t x)
{
    if (x == 0)
        return false;

    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 7);
        if (prev != -1) {
            if ((prev == 0 && d == 5) || (prev == 5 && d == 0))
                return true;
        }
        prev = d;
        x /= 7;
    }
    return false;
}

/* C++ mode: use std::vector to allocate cache of alive objects. */
#ifdef __cplusplus
#include <vector>
#include <new>
static std::vector<seq_t> cacheVec;
#else
/* C mode: ensure adequate capacity using realloc. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort(); /* Allocation failure */
    cache = new_ptr;
    cache_cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined by OEIS, but harmless */

#ifdef __cplusplus
    /* In C++ mode use vector cache. */
    while (cacheVec.size() < n) {
        seq_t k = last_checked + 1;
        if (has_adjacent_0_5(k) && !has_adjacent_0_5(k - 1))
            cacheVec.push_back(k);
        last_checked = k;
    }
    return cacheVec[n - 1];
#else
    /* In C mode use malloc-based array. */
    while (cache_len < n) {
        seq_t k = last_checked + 1;
        if (has_adjacent_0_5(k) && !has_adjacent_0_5(k - 1)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = k;
        }
        last_checked = k;
    }
    return cache[n - 1];
#endif
}
