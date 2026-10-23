/*
 * A189709  Partial sums of A189706.
 * https://oeis.org/A189709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   s(n) = floor((n+1)/3) - s(floor((n+1)/3)) + floor(n/3), s(0)=0
 *            (a(n) is s(n) with offset 1)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values; cache[0] holds s(0) */
    static seq_t *cache = NULL;
    static idx_t cur_size = 0;   /* number of initialized entries */

    /* we need cache[0..n] */
    idx_t needed = n + 1;
    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (cache == NULL) abort();
        /* cur_size stays 0 until we initialize cache[0] */
    } else if (cur_size < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        /* cur_size stays unchanged – only new space is allocated */
    }

    /* initialise base case s(0) = 0 once */
    if (cur_size == 0) {
        cache[0] = (seq_t)0;
    }

    /* compute missing entries in ascending order */
    for (idx_t i = 1; i < needed; ++i) {
        if (i < cur_size) continue;   /* already computed in a previous call */

        idx_t div_up3 = (i + 1) / 3;
        idx_t div3    = i / 3;
        cache[i] = (seq_t)div_up3 - cache[div_up3] + (seq_t)div3;
    }

    cur_size = needed;   /* all indices up to n are now initialized */
    return cache[n];
}
