/*
 * A042557  Denominators of continued fraction convergents to sqrt(807).
 * https://oeis.org/A042557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent to sqrt(807)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Computes a(n): denominators of convergents to sqrt(807). */
seq_t a(idx_t n)
{
    const uint64_t D   = 807;
    const uint64_t A0  = 28;   /* floor(sqrt(807)) */

    /* q_-2 = 1, q_-1 = 0 */
    seq_t q_prev_prev = 1;     /* holds q_{k-1} during iteration */
    seq_t q_prev      = 0;     /* holds q_{k}   during iteration  */

    /* Continued‑fraction variables (always fit in 64 bits) */
    uint64_t m     = 0;
    uint64_t d     = 1;
    uint64_t a_term = A0;      /* current a_k */

    idx_t k = 0;
    while (1) {
        seq_t q_cur = (seq_t)a_term * q_prev + q_prev_prev;
        if (k == n)
            return q_cur;

        /* Shift the recurrence */
        q_prev_prev = q_prev;
        q_prev      = q_cur;

        /* Compute next continued–fraction terms */
        m     = d * a_term - m;
        d     = (D - m * m) / d;
        a_term = (A0 + m) / d;

        ++k;
    }
}
