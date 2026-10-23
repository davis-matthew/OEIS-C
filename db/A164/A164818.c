/*
 * A164818  Integer average digit of squares.
 * https://oeis.org/A164818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 34, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A164818
 *  Formula: a(n) = integer average of the decimal digits of [A164817(n)]^2
 *  Here A164817(n) is the nth positive integer whose square has an integer
 *  average digit.  The function generates A164818(n) on demand and memoizes
 *  the results so that any order of calls is handled efficiently.
 */
#include <stddef.h>
#include "../utils/digit_counts.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) /* offset 1, but undefined below */
        return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms computed */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_x = 1;       /* next integer to test */

    /* grow cache as needed */
    while (cache_len < n) {
        seq_t sq = (seq_t)next_x * (seq_t)next_x;

        unsigned char cnt[10];
        digit_counts(sq, cnt);

        unsigned int len = 0, sum = 0;
        for (int d = 0; d < 10; ++d) {
            len += cnt[d];
            sum += d * cnt[d];
        }

        if (len > 0 && sum % len == 0) {
            /* store the average digit */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)(sum / len);
        }
        ++next_x;
    }

    return cache[n - 1];
}
