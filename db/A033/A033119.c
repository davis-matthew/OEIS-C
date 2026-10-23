/*
 * A033119  Base-9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0.
 * https://oeis.org/A033119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-2) + 9^(n-1), a(1) = 1, a(2) = 9
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;
    static seq_t power_state = 0;

    size_t needed = (size_t)n + 1;          /* array indices 0..n, index 0 unused */
    if (cache == NULL) {
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!cache) return (seq_t)0;
    } else if (cached_max < n) {
        cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!cache) return (seq_t)0;
    }

    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
            power_state = (seq_t)9;          /* exponent for next step (9^1) */
        } else if (i == 2) {
            cache[2] = (seq_t)9;
            power_state = (seq_t)9;          /* 9^1, ready for i=3 */
        } else {
            power_state = power_state * (seq_t)9;   /* 9^(i-1) */
            cache[i] = cache[i-2] + power_state;
        }
    }
    cached_max = n;
    return cache[n];
}
