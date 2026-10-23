/*
 * A040249  Continued fraction for sqrt(266).
 * https://oeis.org/A040249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction coefficients for sqrt(266)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th element of A040249 (continued fraction coefficients for √266) */
seq_t a(idx_t n) {
    /* Store the periodic part of the continued fraction after initialization */
    static seq_t *period = NULL;
    static size_t period_len = 0;

    if (period_len == 0) {
        /* Constants for √266 */
        const idx_t N = 266;
        idx_t a0 = 16;    /* floor(sqrt(266)) */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;

        /* Dynamic array for the period terms */
        size_t cap = 16;
        period = (seq_t*)malloc(cap * sizeof(seq_t));
        period_len = 0;

        /* Generate the period until we hit the terminating term 2*a0 = 32 */
        while (1) {
            /* Next terms of the continued fraction algorithm */
            m = a * d - m;
            d = (N - m * m) / d;
            a = (a0 + m) / d;

            /* Store the new term in the period array */
            if (period_len >= cap) {
                cap <<= 1; /* double capacity */
                period = (seq_t*)realloc(period, cap * sizeof(seq_t));
            }
            period[period_len++] = (seq_t)a;

            if (a == 2 * a0) break;  /* End of the period */
        }
    }

    if (n == 0) return (seq_t)16;  /* a(0) = 16 */

    /* Index into the period for the other terms */
    idx_t idx = (n - 1) % (idx_t)period_len;
    return period[idx];
}
