/*
 * A238047  a(n) is the least number larger than the total number of letters
 *          in the Spanish names for all terms up to and including a(n).
 * https://oeis.org/A238047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   len_num, len_num_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A238047: a(n) is the least integer m such that       \
 * m > S_{n-1} + len(name(m)), where S_{n-1} is the sum of      \
 * lengths of the Spanish names of a(1) … a(n-1).           *
 */

#include <stdlib.h>
#include "../utils/len_num.h"
#include "../utils/len_num_small.h"

/* Cache for computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static seq_t cumulative_sum = 0;
static idx_t last_term = 0;

/* Ensure cache capacity */
static void ensure_cache_cap(idx_t needed)
{
    if (cache_cap < needed) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < needed) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Extend cache up to requested index */
    while (cache_len < n) {
        idx_t m = last_term + 1;   /* start search from previous term + 1 */
        int len;
        for (;;) {
            len = len_num(m);
            if ((seq_t)m > cumulative_sum + (seq_t)len)
                break;
            ++m;
        }
        ensure_cache_cap(cache_len + 1);
        cache[cache_len] = (seq_t)m;
        cumulative_sum += (seq_t)len;
        last_term = m;
        ++cache_len;
    }
    return cache[n - 1];
}
