/*
 * A033318  Least D in the Pellian x^2 - D*y^2 = 1 for which y has least
 *          solution n.
 * https://oeis.org/A033318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 328, u64 n <= 1008,
 *            u128 n <= 1008, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A033318: a(n) = the least positive integer D such that the
   minimal solution of x^2 - D*y^2 = 1 has y = n.  The function
   searches for the smallest D that satisfies this property by
   checking candidates of the form D = (x^2-1)/n^2 and verifying
   that the fundamental solution of the Pell equation for that D
   indeed has y = n.  */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Return the fundamental y for Pell's equation x^2 - D*y^2 = 1.
   If D is a perfect square or no solution exists, return 0.
   The caller must compare the returned value with the desired n. */
static bool is_minimal_y(seq_t D, idx_t n)
{
    seq_t sqrtD = floor_sqrt(D);
    if (sqrtD * sqrtD == D)   /* D is a square, no Pell solution */
        return false;

    seq_t a0 = sqrtD;
    seq_t m  = 0;
    seq_t d  = 1;
    seq_t a  = a0;

    /* Convergents: p_-2 = 0, p_-1 = 1; q_-2 = 1, q_-1 = 0 */
    seq_t p_prev2 = 0;
    seq_t p_prev1 = 1;
    seq_t q_prev2 = 1;
    seq_t q_prev1 = 0;

    for (;;) {
        seq_t p = a * p_prev1 + p_prev2;
        seq_t q = a * q_prev1 + q_prev2;

        /* Check for the fundamental solution */
        if (p * p - D * q * q == 1) {
            return (q == (seq_t)n);
        }

        /* Shift for next iteration */
        p_prev2 = p_prev1;  p_prev1 = p;
        q_prev2 = q_prev1;  q_prev1 = q;

        /* Next terms of the continued fraction */
        seq_t m_next = d * a - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
    }
}

seq_t a(idx_t n)
{
    /* We require n >= 1; behavior for n==0 is irrelevant */
    seq_t nsq = (seq_t)n * (seq_t)n;      /* n^2 */
    seq_t x   = (seq_t)n + 1;             /* start with the first candidate x > n */

    for (;;) {
        seq_t diff = x * x - 1;          /* x^2 - 1 */
        if (diff % nsq == 0) {
            seq_t D = diff / nsq;
            seq_t sD = floor_sqrt(D);
            if (sD * sD != D)           /* skip perfect squares */
                if (is_minimal_y(D, n))
                    return D;
        }
        ++x;                             /* try next x */
    }
}
