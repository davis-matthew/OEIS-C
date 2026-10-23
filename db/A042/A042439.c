/*
 * A042439  Denominators of continued fraction convergents to sqrt(747).
 * https://oeis.org/A042439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(0)=1; a(-1)=0; for n≥1 let
   c = n‑1; if c is odd then use partial quotient 3,
   otherwise use 54. Then a(n)=c’s quotient * a(n‑1)+a(n‑2). */

/* Denominators of continued fraction convergents to sqrt(747). */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Small utility values as seq_t. */
    const seq_t a0   = 27;          /* first partial quotient */
    const seq_t v3   = 3;           /* second partial quotient (odd indices)   */
    const seq_t v54  = 54;          /* third partial quotient (even indices)  */

    /* Keep current and previous two convergents. */
    seq_t p_pp = 0;  /* p_{-2} */
    seq_t p_p  = 1;  /* p_{-1} */
    seq_t q_pp = 1;  /* q_{-2} */
    seq_t q_p  = 0;  /* q_{-1} */
    seq_t q    = 0;  /* current denominator */

    /* iterate through indices 0 … n */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i;            /* partial quotient for index i */
        if (i == 0) {
            a_i = a0;
        } else {
            /* period 3,54 repeats: odd i → 3, even i → 54 */
            a_i = ((i & 1) ? v3 : v54);
        }

        /* recurrence for convergents */
        seq_t p = a_i * p_p + p_pp;
        q      = a_i * q_p + q_pp;

        /* shift for next iteration */
        p_pp = p_p;  p_p = p;
        q_pp = q_p;  q_p = q;
    }

    return q;   /* denominator of nth convergent */
}
