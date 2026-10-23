/*
 * A041608  Numerators of continued fraction convergents to sqrt(322).
 * https://oeis.org/A041608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041608 – Numerators of continued fraction convergents to sqrt(322) */
#include <stdint.h>

/* Returns the n-th numerator of the convergents to sqrt(322).   */
seq_t a(idx_t n)
{
    /* Parameters of sqrt(322) */
    const idx_t A0 = 17;          /* floor(sqrt(322)) */
    const idx_t D  = 322;         /* the radicand */

    /* Continued fraction state variables */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = A0;

    /* Convergent recurrence state:
       h_{-2} = 0, h_{-1} = 1   (initial values for the recurrence)     */
    seq_t h_prev2 = 0;    /* h_{i-2} */
    seq_t h_prev1 = 1;    /* h_{i-1} */

    /* Compute the 0‑th convergent */
    seq_t h_curr = (seq_t)a * h_prev1 + h_prev2;  /* h0 = a0 * 1 + 0 = 17 */
    if (n == 0)
        return h_curr;

    /* Shift to prepare for n >= 1 */
    h_prev2 = h_prev1;      /* now h_prev2 = h_{-1} = 1   */
    h_prev1 = h_curr;       /* now h_prev1 = h0          */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Next state of the continued fraction for sqrt(D)   */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (A0 + m) / d;

        /* Compute the next numerator: h_i = a_i*h_{i-1} + h_{i-2} */
        h_curr = (seq_t)a * h_prev1 + h_prev2;

        if (i == n)
            return h_curr;

        /* Slide the window of previous convergents */
        h_prev2 = h_prev1;
        h_prev1 = h_curr;
    }

    /* This point is never reached; return 0 to satisfy the compiler */
    return 0;
}
