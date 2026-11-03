/*
 * A044703  Numbers n such that string 7,8 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

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

/* Return true if v expressed in base 9 contains the consecutive digits
   7 followed by 8.  Digits are examined from least‑significant to
   most‑significant; we keep track of the previous digit (prev) and
   look for the pair 7 (current) and 8 (previous). */
static bool contains78(idx_t v)
{
    if (v == 0) return false;
    int prev = -1;
    while (v > 0) {
        int d = v % 9;
        v    /= 9;
        if (d == 7 && prev == 8) return true;
        prev = d;
    }
    return false;
}

/* Computes a(n): the nth integer whose base‑9 representation contains
   the substring "78" while the following integer does not. */

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  last_checked = 0;   /* last integer examined */

    if (n == 0) return 0;  /* undefined for offset 1 */

    while (cache_len < n) {
        idx_t cand = last_checked + 1;
        last_checked = cand;

        if (contains78(cand) && !contains78(cand + 1)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }
    return cache[n - 1];
}
