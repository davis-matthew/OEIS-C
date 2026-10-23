/*
 * A032636  a(n) = floor(n^3 / e).
 * https://oeis.org/A032636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^3 / e)
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 28, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation uses continued‑fraction convergents of e.
 * For any n, the floor value lies between the bounds obtained from two
 * consecutive convergents whose denominators exceed 2·n³.  Since the
 * error of a convergent is < 1/(2·q²), this guarantees the bounds differ
 * by at most 1.  We refine until the bounds coincide. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;

    /* n = 0 or 1 → 0 */
    if (n3 == 0) return 0;

    /* Denominator threshold – guarantees error < 1/n³. */
    seq_t limit = (seq_t)2 * n3;

    /* Continued‑fraction initialization for e = [2; 1,2,1,1,4,1,1,6,…] */
    seq_t p_prev2 = 0, p_prev1 = 1;   /* p[-2], p[-1] */
    seq_t q_prev2 = 1, q_prev1 = 0;   /* q[-2], q[-1] */
    seq_t p_curr = 0, q_curr = 0;
    seq_t prev_p = 0, prev_q = 0;

    int idx = 0;                      /* index in the CF expansion */
    while (1) {
        int a_term;
        if (idx == 0) {
            a_term = 2;
        } else {
            int pos = (idx - 1) % 3;
            int k   = (idx - 1) / 3;
            if (pos == 0)       a_term = 1;
            else if (pos == 1)  a_term = 2 * (k + 1);
            else                a_term = 1;
        }

        p_curr = (seq_t)a_term * p_prev1 + p_prev2;
        q_curr = (seq_t)a_term * q_prev1 + q_prev2;

        /* Shift for next iteration */
        p_prev2 = p_prev1;  p_prev1 = p_curr;
        q_prev2 = q_prev1;  q_prev1 = q_curr;

        if (q_curr > limit) {
            prev_p = p_prev2;
            prev_q = q_prev2;

            seq_t lower = (n3 * prev_q) / prev_p;
            seq_t upper = (n3 * q_curr) / p_curr;
            if (lower > upper) { seq_t tmp = lower; lower = upper; upper = tmp; }

            /* Refine until the bounds match. */
            int iter_idx = idx + 1;
            while (lower != upper) {
                int a_next;
                if (iter_idx == 0) {
                    a_next = 2;
                } else {
                    int pos = (iter_idx - 1) % 3;
                    int k   = (iter_idx - 1) / 3;
                    if (pos == 0)       a_next = 1;
                    else if (pos == 1)  a_next = 2 * (k + 1);
                    else                a_next = 1;
                }

                seq_t next_p = (seq_t)a_next * p_curr + prev_p;
                seq_t next_q = (seq_t)a_next * q_curr + prev_q;

                prev_p = p_curr;  prev_q = q_curr;
                p_curr = next_p;  q_curr = next_q;

                lower = (n3 * prev_q) / prev_p;
                upper = (n3 * q_curr) / p_curr;
                if (lower > upper) { seq_t tmp = lower; lower = upper; upper = tmp; }

                iter_idx++;
            }
            return lower;        /* lower == upper */
        }
        idx++;
    }
}
