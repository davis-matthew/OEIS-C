/*
 * A096260  Number of different triangles, squares and rectangles created when
 *          a square piece of paper is folded n times, the first time by a
 *          diagonal of the square and after by the median of the triangle.
 * https://oeis.org/A096260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A096227(n) + A097405(n) + 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (for n > 11):
   a(n) = 30*a(n-2) - 280*a(n-4) + 960*a(n-6) - 1024*a(n-8)   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values a(0) to a(11) */
    static const seq_t init[12] = {
        1ULL, 3ULL, 9ULL, 25ULL, 62ULL, 205ULL, 534ULL,
        2110ULL, 5844ULL, 25996ULL, 75592ULL, 359256ULL
    };

    static seq_t *cache = NULL;      /* dynamic array of computed terms */
    static idx_t cache_len = 0;      /* number of elements stored */

    /* Return a known initial term */
    if (n < 12)
        return init[n];

    /* Allocate initial cache if this is the first call */
    if (cache_len == 0) {
        cache_len = 12;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = init[i];
    }

    /* Extend cache up to requested n */
    if (cache_len <= n) {
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) return 0; /* allocation failure – unlikely in tests */
        cache = new_cache;

        for (idx_t i = cache_len; i <= n; ++i) {
            /* Compute a(i) = 30*a(i-2) - 280*a(i-4) + 960*a(i-6) - 1024*a(i-8) */
            seq_t t_pos1 = cache[i-2] * (seq_t)30;
            seq_t t_pos2 = cache[i-6] * (seq_t)960;
            seq_t sum_pos = t_pos1 + t_pos2;            /* positive part */

            seq_t t_neg1 = cache[i-4] * (seq_t)280;
            seq_t t_neg2 = cache[i-8] * (seq_t)1024;
            seq_t sum_neg = t_neg1 + t_neg2;            /* negative part */

            cache[i] = sum_pos - sum_neg; /* sum_pos >= sum_neg by construction */
        }
        cache_len = n + 1;
    }

    return cache[n];
}
