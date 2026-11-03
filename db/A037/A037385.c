/*
 * A037385  Numbers k such that every base-3 digit of k is a base-9 digit of
 *          k.
 * https://oeis.org/A037385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 6431, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9293 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037385: Numbers k such that every base‑3 digit of k is a base‑9 digit of k. */

#include <stdbool.h>
#include <stdlib.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static bool has_property(seq_t k)
{
    bool has[9] = {false};
    seq_t t = k;

    /* Record base‑9 digits */
    while (t > 0) {
        unsigned d = (unsigned)(t % 9);
        has[d] = true;
        t /= 9;
    }

    /* Check base‑3 digits */
    t = k;
    while (t > 0) {
        unsigned d = (unsigned)(t % 3);
        if (!has[d]) return false;
        t /= 3;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_checked = 1;

    while (cache_len < n) {
        seq_t cand = (seq_t)next_checked;
        if (has_property(cand)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 1 : cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        ++next_checked;
    }
    return cache[n - 1];
}
