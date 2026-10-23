/*
 * A042583  Denominators of continued fraction convergents to sqrt(820).
 * https://oeis.org/A042583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Denominators of continued fraction convergents to sqrt(820). */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)1;

    /* Periodic continued‑fraction coefficients for sqrt(820). */
    static seq_t period[256];
    static size_t period_len = 0;
    static bool period_inited = false;
    if (!period_inited) {
        const unsigned D = 820;
        const unsigned a0 = 28;          /* ⌊√820⌋ */
        unsigned m = 0, d = 1, a = a0;
        period_len = 0;
        while (true) {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = (seq_t)a;
            if (a == 2 * a0) break;
        }
        period_inited = true;
    }

    seq_t prevprev = (seq_t)0;
    seq_t prev = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        seq_t coeff = period[(size_t)(i - 1) % period_len];
        seq_t cur = coeff * prev + prevprev;
        prevprev = prev;
        prev = cur;
    }
    return prev;
}
