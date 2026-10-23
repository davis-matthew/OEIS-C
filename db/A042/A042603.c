/*
 * A042603  Denominators of continued fraction convergents to sqrt(830).
 * https://oeis.org/A042603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 19, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042603: Denominators of convergents of the continued fraction for sqrt(830). */
#include <stdint.h> /* For idx_t type if required */

/* External linkage: function returning the nth term of sequence A042603. */
seq_t a(idx_t n)
{
    /* 830 is the argument of the square‑root, a0 = floor(sqrt(830)) = 28. */
    const idx_t D = 830;
    const idx_t a0 = 28;

    if (n == 0)            /* a(0) = 1 */
        return 1;

    /* Continued‑fraction variables: m, d, current a_k. */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a_k = a0;

    /* Convergent denominators: q_-2 = 1, q_-1 = 0. */
    seq_t q_prev2 = 1;     /* q_{-2} */
    seq_t q_prev1 = 0;     /* q_{-1} */
    seq_t q_cur   = 0;     /* q_k */

    idx_t k;
    for (k = 0; k <= n; ++k) {
        /* Compute q_k = a_k * q_{k-1} + q_{k-2}. */
        q_cur = (seq_t)a_k * q_prev1 + q_prev2;

        if (k == n)
            return q_cur;

        /* Prepare a_{k+1} for next iteration. */
        m = d * a_k - m;
        d = (D - m * m) / d;
        a_k = (a0 + m) / d;

        /* Shift the denominator history. */
        q_prev2 = q_prev1;
        q_prev1 = q_cur;
    }

    /* Should never reach here. */
    return 0;
}
