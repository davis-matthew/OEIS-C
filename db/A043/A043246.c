/*
 * A043246  Numbers k such that 4 and 5 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   brute‑force search for numbers that contain the substring "45"
 *            or "54" in decimal but whose predecessor does not
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has45_or54
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has45_or54.h"

/* Static cache for memoisation */
static seq_t *cache = NULL;      /* cached sequence values */
static idx_t cached_len = 0;    /* number of values cached so far   */
static idx_t cached_cap = 0;    /* current capacity of cache        */
static seq_t next_checked = 0;  /* last integer examined            */

/* Ensure the cache can hold at least 'needed' entries */
static void ensure_cache_size(idx_t needed)
{
    if (needed <= cached_cap) return;
    idx_t newcap = cached_cap ? cached_cap * 2 : 4;
    while (newcap < needed) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = tmp;
    cached_cap = newcap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* unused: offset starts at 1 */
    if (n - 1u >= cached_len) {     /* need more values */
        ensure_cache_size(n);
        while (cached_len < n) {
            ++next_checked;              /* test the next integer */
            if (has45_or54(next_checked) &&
                !has45_or54(next_checked - 1))
            {
                cache[cached_len++] = next_checked;
            }
        }
    }
    return cache[n - 1];
}
