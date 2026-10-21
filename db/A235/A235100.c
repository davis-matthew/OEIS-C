/*
 * A235100  Number of (n+1) X (1+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 2*a(n-2) - 47*a(n-3) + 3*a(n-4) + 84*a(n-5) +
 *            36*a(n-6) with a(1)=154, a(2)=670, a(3)=2900, a(4)=12578,
 *            a(5)=54530, a(6)=236496
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* cache[0] = a(1) … */
static size_t cache_cap = 0;
static idx_t cache_len = 0;

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, defensive */

    /* initialise cache with first six terms once */
    if (cache_len == 0) {
        ensure_capacity(6);
        cache[0] = (seq_t)154;
        cache[1] = (seq_t)670;
        cache[2] = (seq_t)2900;
        cache[3] = (seq_t)12578;
        cache[4] = (seq_t)54530;
        cache[5] = (seq_t)236496;
        cache_len = 6;
    }

    if (n <= cache_len) return cache[n-1];

    /* sliding window of the last six terms: last[0] = a(k-1) */
    seq_t last[6];
    for (int k = 0; k < 6; ++k) {
        last[k] = cache[cache_len - 1 - k];
    }

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* compute next term using unsigned arithmetic */
        seq_t sum_pos = last[0] * 7;
        sum_pos += last[3] * 3;
        sum_pos += last[4] * 84;
        sum_pos += last[5] * 36;

        seq_t sum_neg = last[1] * 2;
        sum_neg += last[2] * 47;

        seq_t next = sum_pos - sum_neg;

        ensure_capacity(cache_len + 1);
        cache[cache_len++] = next;

        /* shift window */
        for (int k = 5; k >= 1; --k) last[k] = last[k-1];
        last[0] = next;
    }

    return cache[n-1];
}
