/*
 * A031178  Periods of sum of cubes of digits iterated until the sequence
 *          becomes periodic.
 * https://oeis.org/A031178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = period length of the sum of cubes of digits iteration
 *            starting from n
 * Range:     u8 n <= 2, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_len.h"

static inline seq_t sum_cubes(seq_t x)
{
    seq_t sum = 0;
    while (x) {
        seq_t d = x % 10;
        sum += d * d * d;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Maximum possible sum of cubes of digits for 64‑bit n:
       at most 729 per decimal digit; 20 digits ⇒ 729*20.  Store a small
       buffer that can index up to that bound. */
    idx_t D = (idx_t)digit_len(n);
    idx_t maxval = D * 730 + 1;          /* allow indices 0 .. D*729 */

    /* Visited array: 0 → not yet visited; else store first step+1. */
    idx_t *vis = (idx_t *)calloc((size_t)maxval, sizeof(idx_t));
    if (!vis) return (seq_t)0;          /* unlikely, but safe fallback */

    seq_t x = n;
    idx_t step = 0;

    while (1) {
        if (x < maxval) {
            idx_t xi = (idx_t)x;
            idx_t first = vis[xi];
            if (first) {
                free(vis);
                /* cycle length = current step – (first occurrence step) */
                return (seq_t)(step - (first - 1));
            }
            vis[xi] = step + 1;         /* record first occurrence */
        }
        ++step;
        x = sum_cubes(x);
    }
}
