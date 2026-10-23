/*
 * A044494  Numbers n such that string 1,5 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer n whose base‑6 representation * contains
 *            the substring "15" but the base‑6 representation of n+1 * does
 *            not. The function searches sequentially and memoises results
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ---------- Check for the substring "15" in base 6 ----------------- */
static bool has_15(seq_t x)
{
    seq_t prev = (seq_t)6;          /* sentinel > 5 for first digit */
    while (x > 0) {
        seq_t rem = x % (seq_t)6;
        /* look for '1' followed immediately by '5' (more significant → less significant) */
        if (rem == (seq_t)1 && prev == (seq_t)5)
            return true;
        prev = rem;
        x /= (seq_t)6;
    }
    return false;
}

/* ----------------- Cache facilities ------------------------------- */

#ifdef __cplusplus
static std::vector<seq_t> cache_vec;
static seq_t next_test = 0;
static void cache_add(seq_t val) { cache_vec.push_back(val); }
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t next_test = 0;
static void cache_add(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        seq_t *p = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) abort();
        cache = p;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}
#endif

/* ------------------- Main sequence function ----------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; value for n=0 is undefined */

#ifdef __cplusplus
    while (cache_vec.size() < n) {
        for (;;) {
            if (has_15(next_test) && !has_15(next_test + 1)) {
                cache_add(next_test);
                ++next_test;
                break;
            }
            ++next_test;
        }
    }
    return cache_vec[n - 1];
#else
    while (cache_len < n) {
        for (;;) {
            if (has_15(next_test) && !has_15(next_test + 1)) {
                cache_add(next_test);
                ++next_test;
                break;
            }
            ++next_test;
        }
    }
    return cache[n - 1];
#endif
}
