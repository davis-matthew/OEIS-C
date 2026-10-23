/*
 * A033438  Number of edges in 6-partite Turán graph of order n.
 * https://oeis.org/A033438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5*n^2/12)
 * Range:     u8 n <= 24, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) + a(n-6) - 2*a(n-7) + a(n-8) */
#include <stddef.h>

seq_t a(idx_t n) {
    /* Initial values a(0) .. a(7) */
    static const seq_t init[8] = {0, 0, 1, 3, 6, 10, 15, 20};

    if (n < 8)            /* return pre‑computed value */
        return init[n];

    /* Work with a sliding window of the last 8 computed values. */
    seq_t prev[8];
    size_t i;
    for (i = 0; i < 8; ++i)
        prev[i] = init[i];

    for (idx_t k = 8; k <= n; ++k) {
        /* Compute a(k) from the recurrence. */
        seq_t val = prev[7] * 2;   /* 2*a(k-1) */
        val -= prev[6];            /* - a(k-2) */
        val += prev[2];            /* + a(k-6) */
        val -= prev[1] * 2;        /* - 2*a(k-7) */
        val += prev[0];            /* + a(k-8) */

        /* Shift the window to the left and insert the new value. */
        for (i = 0; i < 7; ++i)
            prev[i] = prev[i + 1];
        prev[7] = val;
    }

    return prev[7];
}
