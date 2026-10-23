/*
 * A039121  Numbers whose base-10 representation has the same number of 1's
 *          and 3's.
 * https://oeis.org/A039121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth non‑negative integer whose decimal representation
 *            contains an equal number of the digits 1 and 3. (OEIS A039121)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Guard against the undefined offset n = 0. */
    if (n == 0) return (seq_t)0;

    /* Cache of already found terms and state for incremental search. */
    static seq_t *cache     = NULL;      /* array of found terms          */
    static size_t cache_cap = 0;         /* allocated capacity of cache   */
    static size_t cache_len = 0;         /* number of terms currently stored */
    static seq_t next_val    = (seq_t)0; /* next integer to test          */

    /* Grow the cache until it holds the requested term. */
    while (cache_len < (size_t)n) {
        seq_t val = next_val;
        int diff = 0;                    /* +1 for each '1', -1 for each '3' */

        /* Count the digits of val. */
        seq_t t = val;
        if (t == (seq_t)0) {
            diff = 0;                    /* 0 has equal counts by definition */
        } else {
            while (t != (seq_t)0) {
                int d = (int)(t % (seq_t)10);
                if (d == 1)      ++diff;
                else if (d == 3) --diff;
                t = t / (seq_t)10;
            }
        }

        /* If the digit counts are equal, store the value. */
        if (diff == 0) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed – free any resources and give up. */
                    free(cache);
                    cache = NULL;
                    cache_cap = 0;
                    cache_len = 0;
                    return (seq_t)0;
                }
                cache = tmp;
            }
            cache[cache_len++] = val;
        }

        /* Advance to the next integer to test. */
        next_val = next_val + (seq_t)1;
    }

    /* Return the requested term. */
    return cache[n - 1];
}
