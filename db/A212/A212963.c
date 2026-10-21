/*
 * A212963  a(n) = number of ordered triples (w,x,y) such that w,x,y are all
 *          in {0,...,n} and the numbers |w-x|, |x-y|, |y-w| are distinct.
 * https://oeis.org/A212963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 - 3*n^2 - 2*n + 3*(n mod 2))/2 Implemented via
 *            linear recurrence: a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3) +
 *            3*a(n-4) - a(n-5) Base values: a(0)=0, a(1)=0, a(2)=0, a(3)=12,
 *            a(4)=36, a(5)=84
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* base values for n < 6 */
    static const seq_t base[6] = {0, 0, 0, 12, 36, 84};

    if (n < 6) {
        return base[n];
    }

    /* initialize cache with first 6 values if needed */
    if (cache == NULL) {
        cache_len = 6;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0; /* memory failure (unlikely) */
        for (idx_t i = 0; i < 6; ++i) cache[i] = base[i];
    }

    /* extend cache up to n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0; /* memory failure */
        cache = tmp;
        for (idx_t k = cache_len; k <= n; ++k) {
            /* compute a(k) using the recurrence */
            seq_t term_pos = cache[k-1] * (seq_t)3 + cache[k-4] * (seq_t)3;
            seq_t term_neg = cache[k-2] * (seq_t)2 + cache[k-3] * (seq_t)2 + cache[k-5];
            cache[k] = term_pos - term_neg;
        }
        cache_len = new_len;
    }

    return cache[n];
}
