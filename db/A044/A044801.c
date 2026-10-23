/*
 * A044801  Numbers k such that string 8,8 occurs in the base 10
 *          representation of k but not of k+1.
 * https://oeis.org/A044801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer k such that the decimal
 *            representation of k contains the substring "88" but the decimal
 *            representation of k+1 does not
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_88
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_88.h"

/* static cache for already generated terms */
static seq_t *cache = NULL;          /* array of terms */
static idx_t cache_len = 0;          /* number of terms cached */
static idx_t cache_cap = 0;          /* current capacity of cache */

/* current value being examined in the search loop, initialized to 0 */
static seq_t cur_k = 0;

/* grow the cache array to at least new_cap elements */
static void grow_cache(idx_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* external linkage as required */
seq_t a(idx_t n)
{
    if (n == 0)                    /* offset is 1, so n >= 1 */
        abort();

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        ++cur_k;                                   /* next candidate k */

        if (has_88(cur_k) && !has_88(cur_k + (seq_t)1)) {
            if (cache_len == cache_cap) {          /* enlarge cache if full */
                idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
                grow_cache(new_cap);
            }
            cache[cache_len++] = cur_k;
        }
    }
    return cache[n - 1];
}
