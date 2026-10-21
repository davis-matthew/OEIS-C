/*
 * A062899  Number and its reversal are both multiples of 6.
 * https://oeis.org/A062899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A062899: a(n) is the n‑th positive integer m such that
   m is a multiple of 6 and rev(m) is also a multiple of 6.
   Since m is always a multiple of 6, rev(m) is a multiple of 6
   iff its first digit is even.  Hence we select all multiples of
   6 whose most significant decimal digit is even. */
#include <stddef.h>
#include "../utils/rev.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 0;   /* last multiple of 6 examined */

    /* extend cache until we have n terms */
    while (cache_len < (size_t)n) {
        /* next candidate multiple of 6 */
        last_checked += 6;
        idx_t m = last_checked;

        /* rev(m) % 10 gives the most significant digit of m */
        seq_t revm = rev(m);
        uint64_t first_digit = (uint64_t)(revm % 10);

        /* first digit must be even and non‑zero */
        if (first_digit && !(first_digit & 1ULL)) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return 0;      /* allocation failure → undefined */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
        }
    }

    /* return the requested term (1‑based index) */
    return cache[(size_t)n - 1];
}
