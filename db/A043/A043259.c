/*
 * A043259  Numbers k such that 7 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer k whose decimal representation
 *            contains the adjacent digits '79' or '97', but k‑1 does not
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if n has the pair 7,9 or 9,7 in adjacent decimal digits. */
static bool has_pair(seq_t n)
{
    if (n < 10) return false;
    seq_t prev = n % 10;
    n /= 10;
    while (n > 0) {
        seq_t cur = n % 10;
        n /= 10;
        if ((prev == 9 && cur == 7) || (prev == 7 && cur == 9))
            return true;
        prev = cur;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, below is irrelevant */

    /* Static cache for previously found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_to_test = 1;

    /* Build the cache up to the required index. */
    while (cache_len < n) {
        seq_t cur = (seq_t)next_to_test;
        if (has_pair(cur) && !has_pair(cur - 1)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();     /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }
        ++next_to_test;
    }
    return cache[n - 1];
}
