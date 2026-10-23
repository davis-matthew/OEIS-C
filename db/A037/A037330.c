/*
 * A037330  Numbers whose base-6 and base-10 expansions have the same digit
 *          sum.
 * https://oeis.org/A037330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   base6_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/base6_digit_sum.h"

/* Numbers whose base‑6 and base‑10 expansions have the same digit sum.
   The function searches integers sequentially, records those whose
   digit sums match in base 6 and base 10, and returns the nth such
   integer (1‑based).  This implementation memoizes all found terms
   across calls. */

static seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x != 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t cur = 1;

    if (n == 0) return 0;  /* safe placeholder for offset */

    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;  /* allocation failed */
    }

    while (cache_len < n) {
        if (digit_sum(cur) == base6_digit_sum(cur)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
                if (!tmp) {
                    /* allocation failed – return last known term */
                    return cache_len ? cache[cache_len - 1] : 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }
        ++cur;
    }

    return cache[n - 1];
}
