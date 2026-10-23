/*
 * A044168  Numbers n such that string 4,2 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   contains_42
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the next integer n such that its base‑7 representation
   contains the substring "42" while n‑1 does not.  The function keeps a
   simple memoizing cache so that repeated calls in any order run in
   amortised O(1) time per additional term.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains_42.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term of the sequence. */
    if (n == 0) return 0;      /* unused, but keeps the function defined for all n */

    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static seq_t cur = 1;       /* next integer to test */

    if (!initialized) {
        initialized = true;
        cache = NULL;
        cache_len = 0;
        cache_cap = 0;
        cur = 1;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        if (contains_42(cur) && !contains_42(cur - 1)) {
            /* Grow the cache if necessary. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *new_buf = NULL;
#ifdef __cplusplus
                /* C++: use new/delete for arbitrary‑precision types. */
                new_buf = new seq_t[new_cap];
                for (size_t i = 0; i < cache_len; ++i)
                    new_buf[i] = cache[i];
                delete[] cache;
#else
                /* C: use realloc. */
                new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
                cache = new_buf;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }
        ++cur;
    }

    return cache[n - 1];
}
