/*
 * A019512  Expansion of 1/((1-4*x)*(1-7*x)*(1-8*x)).
 * https://oeis.org/A019512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A019512:
 * a(n) = (3*8^(n+2)-4*7^(n+2)+4^(n+2))/12.
 * Recurrence: a(0)=1, a(1)=19, a(2)=245;
 * for n>=3   a(n)=19*a(n-1)-116*a(n-2)+224*a(n-3).
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* Ensure we can hold indices 0..n inclusive. */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute sequence up to n (memoizing). */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)19;
        } else if (i == 2) {
            cache[i] = (seq_t)245;
        } else {
            seq_t t1 = cache[i-1] * (seq_t)19;
            seq_t t2 = cache[i-2] * (seq_t)116;
            seq_t t3 = cache[i-3] * (seq_t)224;
            cache[i] = t1 - t2 + t3;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
