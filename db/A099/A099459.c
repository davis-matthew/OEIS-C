/*
 * A099459  Expansion of 1/(1 - 7*x + 9*x^2).
 * https://oeis.org/A099459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*a(n-1) - 9*a(n-2), a(0)=1, a(1)=7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* realloc failed – unlikely */
        cache = tmp;
        cache_size = new_size;

        /* Initialize base cases if they are not yet present */
        if (old_size == 0) {
            if (new_size > 0) cache[0] = (seq_t)1;
        }
        if (old_size <= 1) {
            if (new_size > 1) cache[1] = (seq_t)7;
        }

        /* Compute new terms using the recurrence */
        idx_t start = (old_size > 1) ? old_size : 2;
        for (idx_t i = start; i < new_size; ++i) {
            seq_t val = cache[i - 1];
            val = val * (seq_t)7;          /* 7 * a(n-1) */
            seq_t sub = cache[i - 2];
            sub = sub * (seq_t)9;          /* 9 * a(n-2) */
            val -= sub;                    /* a(n) = 7*a(n-1) - 9*a(n-2) */
            cache[i] = val;
        }
    }

    return cache[n];
}
