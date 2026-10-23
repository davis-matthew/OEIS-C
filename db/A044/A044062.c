/*
 * A044062  Numbers k such that string 0,1 occurs in the base 4 representation
 *          of k but not of k-1.
 * https://oeis.org/A044062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 4356, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that the base‑4 representation of k contains the
   consecutive digits 0 followed by 1 but the representation of k‑1 does
   not.  This implementation scans k in order, caching found values
   so that calls to a(idx_t n) can be made in any order. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Return true if the base‑4 representation of x contains the digit
   sequence 0,1 (msd → lsd).  The routine scans the digits from
   least‑significant upward; in that order the pattern 0,1 corresponds
   to 1,0, so we test for previous digit == 1 and current digit == 0. */
static bool has_01(seq_t x)
{
    if (x == (seq_t)0)
        return false;

    seq_t prev = x % (seq_t)4;
    x = x / (seq_t)4;

    while (x > (seq_t)0) {
        seq_t cur = x % (seq_t)4;
        if (prev == (seq_t)1 && cur == (seq_t)0)
            return true;
        prev = cur;
        x = x / (seq_t)4;
    }
    return false;
}

/* Return the nth term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for 0, but return 0 */

    /* Static cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = (seq_t)1;

    while (cache_len < (size_t)n) {
        if (has_01(next_candidate) && !has_01(next_candidate - (seq_t)1)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : (cache_cap << 1);
                seq_t *new_cache;
#ifdef __cplusplus
                new_cache = new seq_t[new_cap];
                for (size_t i = 0; i < cache_len; ++i)
                    new_cache[i] = cache[i];
                delete[] cache;
#else
                new_cache = realloc(cache, new_cap * sizeof(seq_t));
#endif
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[(size_t)(n - 1)];
}
