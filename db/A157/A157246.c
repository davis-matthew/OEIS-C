/*
 * A157246  Positive numbers y such that y^2 is of the form x^2+(x+343)^2 with
 *          integer x.
 * https://oeis.org/A157246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-7) - a(n-14) for n > 14 Initial values: a(1)=245,
 *            a(2)=265, a(3)=287, a(4)=343, a(5)=427, a(6)=493, a(7)=637,
 *            a(8)=833, a(9)=1097, a(10)=1295, a(11)=1715, a(12)=2275,
 *            a(13)=2693, a(14)=3577
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static storage for memoisation */
static seq_t *cache = NULL;
static size_t cached_max = 0;

/* Constant initial values, cast to seq_t */
static const seq_t init_vals[14] = {
    (seq_t)245, (seq_t)265, (seq_t)287, (seq_t)343, (seq_t)427,
    (seq_t)493, (seq_t)637, (seq_t)833, (seq_t)1097, (seq_t)1295,
    (seq_t)1715, (seq_t)2275, (seq_t)2693, (seq_t)3577
};

extern seq_t a(idx_t n)
{
    /* a(0) is undefined; return 0 for safety */
    if (n == 0)
        return 0;

    size_t idx = (size_t)n;   /* safe cast for n up to reasonable limits */

    /* Extend cache if needed */
    if (cached_max < idx) {
        size_t new_len = idx + 1;   /* we ignore index 0 for simplicity */
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – return 0 as a fallback */
            return 0;
        }
        cache = new_cache;

        /* Compute new entries */
        for (size_t i = cached_max + 1; i <= idx; ++i) {
            if (i <= 14) {
                cache[i] = init_vals[i - 1];
            } else {
                /* recurrence: a(n) = 6*a(n-7) - a(n-14) */
                cache[i] = cache[i - 7] * (seq_t)6 - cache[i - 14];
            }
        }
        cached_max = idx;
    }

    return cache[idx];
}
