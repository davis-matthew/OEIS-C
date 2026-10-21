/*
 * A219794  First differences of 5-smooth numbers (A051037).
 * https://oeis.org/A219794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051037(n+1) - A051037(n) (first differences of 5‑smooth
 *            numbers)
 * Range:     u8 n <= 49, u16 n <= 634, u32 n <= 2256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the OEIS offset (not expected). */
    if (n < 1) return 0;

    /* Static state to cache 5‑smooth numbers and generation indices. */
    static seq_t *smooth = NULL;          /* array of generated smooth numbers */
    static size_t  smooth_cap = 0;        /* allocated capacity */
    static idx_t   size = 0;              /* number of elements currently stored   */
    static idx_t   i2 = 0, i3 = 0, i5 = 0;/* next multipliers indices */

    idx_t needed = n + 1;                 /* we need indices 0 … n inclusive */

    /* Ensure the array is large enough. */
    if (smooth_cap < needed) {
        size_t new_cap = smooth_cap ? smooth_cap * 2 : 1;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(smooth, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* out of memory */
        smooth = tmp;
        smooth_cap = new_cap;
    }

    /* Grow the 5‑smooth sequence to the required length. */
    if (size < needed) {
        /* If this is the first call, initialise the first element. */
        if (size == 0) {
            smooth[0] = 1;
            size = 1;
            i2 = i3 = i5 = 0;
        }
        /* Continue generating from the current state. */
        for (; size < needed; ++size) {
            seq_t v2 = smooth[i2] * 2;
            seq_t v3 = smooth[i3] * 3;
            seq_t v5 = smooth[i5] * 5;
            seq_t next = v2;
            if (v3 < next) next = v3;
            if (v5 < next) next = v5;
            smooth[size] = next;
            if (next == v2) ++i2;
            if (next == v3) ++i3;
            if (next == v5) ++i5;
        }
    }

    /* Return the difference between consecutive 5‑smooth numbers. */
    return smooth[n] - smooth[n - 1];
}
