/*
 * A225387  Construct sequences P,Q,R by the rules: Q = first differences of
 *          P, R = second differences of P, P starts with 1,3,9, Q starts with
 *          2,6, R starts with 4; at each stage the smallest number not yet
 *          present in P,Q,R is appended to R. Sequence gives R.
 * https://oeis.org/A225387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   add_present
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Generative algorithm:  Build sequences P, Q, R.  P starts 1,3,9,
    Q = first differences of P (2,6), R = second differences of P (4).
    At each step, append the smallest positive integer not yet present
    in any of P, Q or R to R.  The function returns the a(n)-th term
    of R (offset 1).  It works for any seq_t size and for up to
    n ≈ 10000 within the time limits. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/add_present.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* out‑of‑range input, but not used */
        return 0;

    /* store which positive integers have already appeared */
    uint8_t *present = NULL;
    size_t present_sz = 0;

    /* seed initial numbers */
    add_present(1, &present, &present_sz);  /* P[0] */
    add_present(3, &present, &present_sz);  /* P[1] */
    add_present(9, &present, &present_sz);  /* P[2] */

    add_present(2, &present, &present_sz);  /* Q[0] */
    add_present(6, &present, &present_sz);  /* Q[1] */

    add_present(4, &present, &present_sz);  /* R[0] */

    /* indices of the last two elements of P */
    seq_t p_prevprev = 3;   /* P[1] */
    seq_t p_prev    = 9;    /* P[2] */

    seq_t last_r = 4;       /* last term appended to R */
    idx_t r_count = 1;      /* number of R terms computed */

    if (n == 1) {
        free(present);
        return last_r;
    }

    /* smallest positive integer not yet present */
    uint64_t cand = 1;
    while (cand < present_sz && present[(size_t)cand])
        ++cand;

    while (r_count < n) {
        seq_t r_val = (seq_t)cand;
        add_present(r_val, &present, &present_sz);    /* mark R term */

        /* compute next P and Q values */
        seq_t new_p = r_val + (p_prev << 1) - p_prevprev;   /* P next */
        seq_t new_q = new_p - p_prev;                      /* Q next */

        add_present(new_p, &present, &present_sz);
        add_present(new_q, &present, &present_sz);

        /* move window for P */
        p_prevprev = p_prev;
        p_prev    = new_p;

        last_r = r_val;
        ++r_count;

        /* find next missing integer */
        ++cand;
        while (cand < present_sz && present[(size_t)cand])
            ++cand;
    }

    free(present);
    return last_r;
}
