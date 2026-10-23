/*
 * A041164  Numerators of continued fraction convergents to sqrt(92).
 * https://oeis.org/A041164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = p_n where p_-1=1, p_0=9, and for n>=1, p_n = a_n *
 *            p_(n-1) + p_(n-2); a_n are the partial‑quotients of the
 *            continued fraction of sqrt(92)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
 
seq_t a(idx_t n)
{
    const uint64_t D = 92;          /* square‑free part of D */
    const unsigned int a0 = 9;      /* floor(sqrt(D)) */
 
    if (n == 0) return (seq_t)a0;
 
    /* Variables for the recurrence of the continued fraction of sqrt(D) */
    uint64_t m = 0;                 /* m_{k-1} */
    uint64_t d = 1;                 /* d_{k-1} */
    unsigned int a = a0;            /* a_{k-1} (initially a0) */
 
    /* Numerators of convergents: p_{-1}=1, p_0=a0 */
    seq_t p_prev_prev = (seq_t)1;   /* p_{-1} */
    seq_t p_prev = (seq_t)a0;       /* p_0 */
    seq_t p_curr;
 
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute next partial quotient a_i using the standard algorithm */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (unsigned int)((a0 + m) / d);
 
        /* Recurrence for numerator p_i */
        p_curr = (seq_t)a * p_prev + p_prev_prev;
 
        /* Shift for next iteration */
        p_prev_prev = p_prev;
        p_prev = p_curr;
    }
 
    return p_prev;
}
