/*
 * A147526  Numbers Y such that 309*Y^2 + 103 is a square.
 * https://oeis.org/A147526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   The sequence a(n) consists of the y‑coordinates of solutions of
 *            x^2 – 309 y^2 = 103. Let (xU , yU) be the fundamental solution
 *            of the Pell equation x^2 – 309 y^2 = 1. Then for k ≥ 0 (xk , yk)
 *            = (x0 , y0) · (xU + yU√309)^k Consequently the y‑sequence
 *            satisfies the linear recurrence y(k+1) = 2·xU ·...
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t get_coeff(void) {
    static seq_t coeff = 0;
    static int initialized = 0;
    if (initialized) return coeff;

    const seq_t D = 309;               /* D in the Pell equation */
    const seq_t a0 = 17;               /* floor(sqrt(309)) */

    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    /* convergents p/q */
    seq_t p0 = a0, p1 = 1;   /* p(-1)=1 , p0=a0   */
    seq_t q0 = 1,  q1 = 0;   /* q(-1)=0 , q0=1    */

    seq_t p2, q2;
    seq_t m_next, d_next, a_next;

    /* iterate continued‑fraction steps until p^2 – D q^2 = 1 */
    while (1) {
        m_next = d * a - m;
        d_next = (D - m_next * m_next) / d;
        a_next = (a0 + m_next) / d_next;

        /* next convergent */
        p2 = a_next * p0 + p1;
        q2 = a_next * q0 + q1;

        /* test Pell equation */
        seq_t lhs = p2 * p2 - D * q2 * q2;
        if (lhs == 1) {
            coeff = 2 * p2;          /* coeff = 2·xU */
            break;
        }

        /* shift for next step */
        m  = m_next;
        d  = d_next;
        a  = a_next;
        p1 = p0;
        p0 = p2;
        q1 = q0;
        q0 = q2;
    }

    initialized = 1;
    return coeff;
}

seq_t a(idx_t n) {
    /* Handle the trivial cases directly */
    if (n == 1) return (seq_t)103443;
    if (n == 2) return (seq_t)13282645066862013ULL;

    /* Obtain the recurrence coefficient */
    seq_t coeff = get_coeff();

    /* Compute up to the requested index using the recurrence
       y(k+1) = coeff·y(k) – y(k–1) */
    idx_t i;
    seq_t y_prevprev = (seq_t)103443;                      /* y(1) */
    seq_t y_prev     = (seq_t)13282645066862013ULL;        /* y(2) */
    seq_t y_next = 0;

    for (i = 3; i <= n; ++i) {
        y_next = coeff * y_prev - y_prevprev;
        y_prevprev = y_prev;
        y_prev = y_next;
    }

    return y_prev;
}
