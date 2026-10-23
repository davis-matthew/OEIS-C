/*
 * A042718  Numerators of continued fraction convergents to sqrt(889).
 * https://oeis.org/A042718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of the continued fraction
 *            of sqrt(889)
 * Range:     u8 none, u16 n <= 8, u32 n <= 19, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A straightforward implementation using the recurrence
   p_{-1}=1, p_0=a0,  p_n = a_n * p_{n-1} + p_{n-2}
   where a_n are the partial quotients of sqrt(889).  The partial
   quotients are periodic with period 42; here they are computed
   on first use and stored in a static table.  The function
   works for any n>=0, and uses only integer arithmetic.  */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* constants for sqrt(889) */
    const seq_t D      = 889;          /* the number under the square root */
    const seq_t a0     = 29;           /* floor(sqrt(D)) */
    const seq_t two_a0 = a0 + a0;      /* termination value of the period */

    /* static storage for the partial quotients (max period 128 – plenty) */
    static seq_t coeff[128];
    static size_t L = 0;        /* length of the period */
    static int init_done = 0;   /* 0 until coeffs are computed */

    /* compute the partial quotients once */
    if (!init_done) {
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_cur = a0;       /* current partial quotient */
        size_t count = 0;

        do {
            /* generate next partial quotient */
            m = d * a_cur - m;
            d = (D - m * m) / d;
            a_cur = (a0 + m) / d;

            coeff[count++] = a_cur;
        } while (a_cur != two_a0);   /* period ends when a_n == 2*a0 */

        L = count;
        init_done = 1;
    }

    /* special case: n == 0 */
    if (n == 0)
        return a0;

    /* convergent recurrence */
    seq_t p_prev2 = 1;   /* p_{-1} */
    seq_t p_prev1 = a0;  /* p_0 */
    seq_t p_curr;

    for (idx_t i = 1; i <= n; ++i) {
        size_t idx = (size_t)(i - 1) % L; /* a_i = coeff[idx] */
        seq_t a_i = coeff[idx];
        p_curr = a_i * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    return p_prev1;   /* p_n is the nth numerator */
}
