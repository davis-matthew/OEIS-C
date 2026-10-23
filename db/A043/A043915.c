/*
 * A043915  Numbers k such that 0 and 5 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043915
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

/*
 * OEIS A043915: Numbers k such that the digits 0 and 5 occur
 * consecutively (in either order) in the base‑7 representation of
 * k, while the base‑7 representation of k+1 contains no such pair.
 *
 * The function builds the sequence incrementally; previously found
 * terms are cached so that queries for any n (in any order) are fast.
 */
#include <stdlib.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
#include <vector>
#endif

/* Return true iff x has adjacent digits 0 and 5 in its base‑7 representation. */
static bool has_adj05(seq_t x)
{
    bool prev0 = false, prev5 = false;
    while (x > 0) {
        unsigned digit = (unsigned)(x % (seq_t)7);
        if ((prev0 && digit == 5) || (prev5 && digit == 0))
            return true;
        prev0 = (digit == 0);
        prev5 = (digit == 5);
        x = x / (seq_t)7;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* value for offset 0 is irrelevant */

#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static seq_t next_k = 0;

    while (cache.size() < n) {
        seq_t k = next_k;
        for (;; ++k) {
            if (has_adj05(k) && !has_adj05(k + 1)) {
                cache.push_back(k);
                next_k = k + 1;
                break;
            }
        }
    }
    return cache[n - 1];
#else
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_k = 0;

    while (cache_len < n) {
        seq_t k = next_k;
        for (;; ++k) {
            if (has_adj05(k) && !has_adj05(k + 1)) {
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    if (new_cap < cache_len + 1) new_cap = cache_len + 1;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                next_k = k + 1;
                break;
            }
        }
    }
    return cache[n - 1];
#endif
}
