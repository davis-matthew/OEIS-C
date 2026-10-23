/*
 * A041896  Numerators of continued fraction convergents to sqrt(470).
 * https://oeis.org/A041896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of nth convergent of √470. Recurrence: a(-1) =
 *            1, a(0) = 21. For n ≥ 1, a(n) = A[n] * a(n-1) + a(n-2), where
 *            A[n] is the periodic sequence {1, 2, 8, 2, 1, 42} repeating
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t period[6] = {1, 2, 8, 2, 1, 42};

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)21;
    seq_t p_prevprev = (seq_t)1;          /* a(-1) */
    seq_t p_prev     = (seq_t)21;         /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        uint8_t term = period[(i - 1) % 6];           /* a_i */
        seq_t cur = (seq_t)term * p_prev + p_prevprev;/* a(i) */
        p_prevprev = p_prev;
        p_prev = cur;
    }
    return p_prev;
}
