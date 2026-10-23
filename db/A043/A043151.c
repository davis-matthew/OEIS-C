/*
 * A043151  Numbers k such that 5 and 6 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_adj_56
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Algorithm: Linear search for numbers k such that the base‑7 representation
 * of k contains the digit pair 5 and 6 adjacent in either order (56 or 65),
 * while the base‑7 representation of k–1 does not.
 *
 * The function caches found values so repeated calls are fast.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_adj_56.h"

/* Ensure capacity in a dynamic array of seq_t */
static void ensure_capacity(size_t needed, seq_t **arr, size_t *cap)
{
    if (*cap < needed) {
        size_t newcap = (*cap == 0) ? 16 : *cap << 1;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *arr = tmp;
        *cap = newcap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 0;

    if (n < 1) return 0;               /* undefined below offset */

    while (cache_len < (size_t)n) {
        seq_t cur = last_checked + 1;
        last_checked = cur;

        if (has_adj_56(cur) && !has_adj_56(cur - 1)) {
            ensure_capacity(cache_len + 1, &cache, &cache_cap);
            cache[cache_len++] = cur;
        }
    }
    return cache[n - 1];
}
