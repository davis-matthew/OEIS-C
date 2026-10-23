/*
 * A043107  Numbers k such that 0 and 2 occur juxtaposed in the base-5
 *          representation of k but not of k-1.
 * https://oeis.org/A043107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 4210, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n):  the nth positive integer whose base‑5 expansion contains an
 * adjacent pair 02 or 20, while the preceding integer does not.
 * Computed by a linear search with memoisation.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Check whether a base‑5 representation of x contains the pattern
 * 02 or 20 in any adjacent pair of digits. */
static bool has_adj02(seq_t x)
{
    if (x < 5) return false;        /* a single digit cannot contain a pair */
    while (x >= 5) {
        seq_t cur  = x % 5;
        seq_t next = (x / 5) % 5;
        if ((cur == 0 && next == 2) || (cur == 2 && next == 0))
            return true;
        x /= 5;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, harmless */

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t last_checked = 0; /* last integer that was examined */

    /* Grow capability if necessary */
    while (cache_len < n) {
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure: return 0 as safe value */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Scan next integer */
        last_checked++;
        seq_t cur = (seq_t)last_checked;
        if (has_adj02(cur) && !has_adj02((seq_t)(last_checked - 1)))
            cache[cache_len++] = cur;
    }

    return cache[n - 1];
}
