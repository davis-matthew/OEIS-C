/*
 * A189403  n+[nr/s]+[nt/s]; r=1, s=e, t=e^2.
 * https://oeis.org/A189403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/e⌋ + ⌊n·e⌋
 * Range:     u8 n <= 62, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn        = (seq_t)n;     /* n converted to seq_t        */
    seq_t floor_mul = 0;            /* floor(n·e)                  */
    seq_t floor_div = 0;            /* floor(n/e)                  */
    bool   found_mul = false, found_div = false;

    /* Convergents of e: p/q.  p/q alternates below/above e. */
    seq_t p_prevprev = 0,  p_prev = 1;
    seq_t q_prevprev = 1,  q_prev = 0;
    seq_t low_p = 0,  low_q = 1;   /* convergent <= e */
    seq_t high_p = 0, high_q = 1;  /* convergent >  e */

    bool below = true;  /* the next convergent will be below e */

    for (idx_t i = 0; ; ++i)
    {
        /* continued‑fraction coefficients of e:
           a0 = 2; a1 = 1, a2 = 2, a3 = 1, a4 = 1, a5 = 4, a6 = 1, a7 = 1, a8 = 6, ...  */
        int a;
        if (i == 0) {
            a = 2;
        } else {
            idx_t k = i - 1;          /* 1‑based index starting from the first block */
            int  m = (int)(k % 3);
            if (m == 0)      a = 1;
            else if (m == 1) a = 2 * ((int)(k / 3) + 1);
            else             a = 1;
        }

        /* next convergent:  p = a·p_prev + p_prevprev   ,   q = a·q_prev + q_prevprev   */
        seq_t p = (seq_t)a * p_prev + p_prevprev;
        seq_t q = (seq_t)a * q_prev + q_prevprev;

        if (below) {
            low_p = p;
            low_q = q;
        } else {
            high_p = p;
            high_q = q;

            /* floor(n·e) from lower and upper convergents      */
            seq_t low_mul  = (nn * low_p)  / low_q;
            seq_t high_mul = (nn * high_p) / high_q;
            if (!found_mul && low_mul == high_mul) {
                floor_mul = low_mul;
                found_mul = true;
            }

            /* floor(n/e) from reciprocal bounds */
            /* lower bound:  q_high / p_high   (p_high > e) */
            seq_t low_div  = (nn * high_q) / high_p;
            /* upper bound:  q_low  / p_low   (p_low  < e) */
            seq_t high_div = (nn * low_q)  / low_p;
            if (!found_div && low_div == high_div) {
                floor_div = low_div;
                found_div = true;
            }

            if (found_mul && found_div)
                break;
        }

        /* shift for next iteration */
        p_prevprev = p_prev;
        p_prev     = p;
        q_prevprev = q_prev;
        q_prev     = q;
        below       = !below;
    }

    return nn + floor_mul + floor_div;
}
