/*
 * A050423  Numbers for which in base 2 the least number of digits that can be
 *          removed to leave a palindrome (possibly beginning with 0) is 4.
 * https://oeis.org/A050423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   bitlen_idx_t, lps_min_del
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A050423: Numbers for which in base 2 the least number of digits that can be 
 * removed to leave a palindrome (possibly beginning with 0) is 4. 
 * 
 * The implementation searches forward for numbers with exactly 4 deletions
 * needed to make their binary representation a palindrome.  The results
 * are cached so the function works in any call order.  The algorithm
 * uses a straightforward O(L^2) dynamic‑programming routine for each
 * number where L is its bit length (≤ 64 because idx_t is 64‑bit).
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bitlen_idx_t.h"
#include "../utils/lps_min_del.h"

/* Cached list of found terms. */
static seq_t *cache   = NULL;
static size_t cacheSize = 0;   /* number of terms stored */
static size_t cacheCap  = 0;   /* allocated capacity */
static idx_t lastChecked = 0;  /* largest integer examined so far */

/* Ensure that the cache has capacity for at least `newCap` elements. */
static void grow_cache(size_t newCap)
{
    if (newCap <= cacheCap) return;
    size_t newSize = newCap > 16 ? newCap : 16;
    if (cache == NULL) {
        cache = (seq_t *)malloc(newSize * sizeof(seq_t));
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
        cache = tmp;
    }
    cacheCap = newSize;
}

/* Return the n-th term of A050423 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless */

    /* Generate terms until we have at least n entries. */
    while (cacheSize < n) {
        idx_t cur = lastChecked + 1;
        while (true) {
            if (lps_min_del(cur) == 4) {
                grow_cache(cacheSize + 1);
                cache[cacheSize++] = (seq_t)cur;
                lastChecked = cur;
                break;
            }
            ++cur;
        }
    }

    return cache[n - 1];
}
