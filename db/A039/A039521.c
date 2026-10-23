/*
 * A039521  Numbers whose base-12 representation has the same nonzero number
 *          of 2's and 10's.
 * https://oeis.org/A039521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   sequence a(n) is the n-th integer whose base‑12 representation
 *            contains the same positive number of the digit 2 and the digit
 *            10
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store computed terms, supports arbitrary ordering of calls */
    static seq_t *cache = NULL;      /* array of computed terms */
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* capacity of the cache */
    static seq_t last_checked = 0;   /* last number examined */

    /* ensure cache has at least n elements */
    while (cache_len < n) {
        /* advance to next candidate */
        last_checked = last_checked + 1;

        /* test candidate */
        seq_t tmp = last_checked;
        int cnt2 = 0, cnt10 = 0;

        while (tmp != 0) {
            unsigned digit = (unsigned)(tmp % 12);
            if (digit == 2)      cnt2  += 1;
            else if (digit == 10) cnt10 += 1;
            tmp = tmp / 12;
        }

        if (cnt2 > 0 && cnt10 > 0 && cnt2 == cnt10) {
            /* store the found term */
            if (cache_len == cache_cap) {
                /* grow capacity */
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort(); /* allocation failure */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len] = last_checked;
            cache_len += 1;
        }
    }

    /* return the n-th term (1‑based index) */
    return cache[n - 1];
}
