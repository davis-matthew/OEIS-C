/*
 * A041552  Numerators of continued fraction convergents to sqrt(294).
 * https://oeis.org/A041552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(294) .
   Recurrence: a(0)=17, a(1)=103, and for n≥2
   a(n) = q(n)*a(n-1) + a(n-2),  
   where the partial quotients q(n) come from the periodic
   continued‑fraction of sqrt(294): q(0)=17, q(1)=6, q(2)=1,
   q(3)=4, q(4)=1, q(5)=6, q(6)=34, and thereafter repeating
   every 6 terms.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for sqrt(294) */
    const uint64_t D   = 294;
    const uint64_t a0  = 17;          /* floor(sqrt(294)) */

    /* State for the continued‑fraction algorithm */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_q = a0; /* current partial quotient q(n) */

    /* Recurrence state for the numerators */
    seq_t p_prev2 = 0;   /* a(-2) = 0  */
    seq_t p_prev1 = 1;   /* a(-1) = 1  */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t p_k = (seq_t)a_q * p_prev1 + p_prev2;
        if (k == n)
            return p_k;

        /* Advance to next partial quotient using the standard
           sqrt(D) continued‑fraction recurrence. */
        m = d * a_q - m;
        d = (D - m * m) / d;
        a_q = (a0 + m) / d;

        /* Shift recurrence state */
        p_prev2 = p_prev1;
        p_prev1 = p_k;
    }

    /* Should never reach here */
    return 0;
}
