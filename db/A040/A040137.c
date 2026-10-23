/*
 * A040137  Continued fraction for sqrt(150).
 * https://oeis.org/A040137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction for sqrt(150). a(0) = ⌊√150⌋ = 12,
 *            a(1),a(2),… are the repeating part: 4,24,4,24,… (period length
 *            2)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root for unsigned 64‑bit integers. */
static idx_t floor_sqrt_uint64(idx_t n) {
    idx_t low = 0, high = (n >> 1) + 1;
    while (low <= high) {
        idx_t mid = (low + high) / 2;
        if (mid * mid <= n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n) {
    /* Static state to memoise the period. */
    static bool initialized = false;
    static seq_t a0;                 /* floor(√150) */
    static seq_t *period = NULL;     /* periodic terms (≥0) */
    static idx_t period_len = 0;     /* length of the period */

    if (!initialized) {
        /* --- Compute the continued‑fraction period for √150 --- */
        a0 = (seq_t)floor_sqrt_uint64(150);  /* a0 = 12 */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_prev = a0;                     /* a0 of the algorithm */

        /* Allocate temporary storage for the period. */
        size_t cap = 4;
        size_t len = 0;
        period = (seq_t *)malloc(cap * sizeof(seq_t));

        while (true) {
            /* Next values of m,d,a_. */
            m = d * a_prev - m;
            d = (150 - m * m) / d;            /* 150 is the radicand */
            seq_t a_next = (a0 + m) / d;

            /* Store a_next. */
            if (len == cap) {
                cap *= 2;
                period = (seq_t *)realloc(period, cap * sizeof(seq_t));
            }
            period[len++] = a_next;

            /* Period ends when a_next == 2*a0. */
            if (a_next == a0 * 2) {
                period_len = len;
                period = (seq_t *)realloc(period,
                                           period_len * sizeof(seq_t));
                break;
            }
            a_prev = a_next;
        }
        initialized = true;
    }

    /* --- Return the required term --- */
    if (n == 0) return a0;
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
