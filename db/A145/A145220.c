/*
 * A145220  a(n) is the number of even permutations (of an n-set) with exactly
 *          2 fixed points.
 * https://oeis.org/A145220
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n,2) * ( ! (n-2) + D_s(n-2) ) / 2, where !m is the
 *            derangement number and D_s(m) is the signed sum of signs of
 *            derangements of m. D_s satisfies: D_s(0)=1, D_s(1)=0, D_s(k)=
 *            -(k-1) * ( D_s(k-1)+D_s(k-2) )
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    idx_t m = n - 2;            /* m = n-2 */

    /* derangement numbers d(k) with d(0)=1, d(1)=0 */
    seq_t d_prev2 = (seq_t)1;   /* d(0) */
    seq_t d_prev1 = (seq_t)0;   /* d(1) */

    /* signed D_s(k) stored as magnitude + sign flag */
    seq_t Dmag_prev2 = (seq_t)1;   /* |D_s(0)| = 1, sign positive */
    bool    Dsign_prev2 = true;    /* D_s(0) = +1 */

    seq_t Dmag_prev1 = (seq_t)0;   /* |D_s(1)| = 0, sign irrelevant */
    bool    Dsign_prev1 = false;   /* D_s(1) = 0 */

    seq_t d_k, Dmag_k, d_temp, Dmag_temp;
    bool    Dsign_k, Dsign_temp;

    for (idx_t k = 2; k <= m; ++k) {
        /* compute d(k) = (k-1)*(d(k-1)+d(k-2)) */
        d_k = (seq_t)(k - 1) * (d_prev1 + d_prev2);

        /* compute D_s(k) */
        /* first sum D_s(k-1)+D_s(k-2) */
        if (Dsign_prev1 == Dsign_prev2) {
            /* same sign: magnitudes add */
            Dmag_temp = Dmag_prev1 + Dmag_prev2;
            Dsign_temp = Dsign_prev1;
        } else {
            /* signs differ: magnitudes subtract */
            if (Dmag_prev1 >= Dmag_prev2) {
                Dmag_temp = Dmag_prev1 - Dmag_prev2;
                Dsign_temp = Dsign_prev1;
            } else {
                Dmag_temp = Dmag_prev2 - Dmag_prev1;
                Dsign_temp = Dsign_prev2;
            }
        }
        /* D_s(k) has opposite sign of the sum, and magnitude multiplied by (k-1) */
        Dsign_k = !Dsign_temp;
        Dmag_k  = (seq_t)(k - 1) * Dmag_temp;

        /* shift for next iteration */
        d_prev2 = d_prev1;
        d_prev1 = d_k;

        Dmag_prev2 = Dmag_prev1;
        Dsign_prev2 = Dsign_prev1;
        Dmag_prev1 = Dmag_k;
        Dsign_prev1 = Dsign_k;
    }

    /* final values */
    seq_t d_final;
    seq_t Dmag_final;
    bool    Dsign_final;

    if (m == 0) {
        d_final = (seq_t)1;
        Dmag_final = (seq_t)1;
        Dsign_final = true;
    } else if (m == 1) {
        d_final = (seq_t)0;
        Dmag_final = (seq_t)0;
        Dsign_final = false;
    } else {
        d_final = d_prev1;
        Dmag_final = Dmag_prev1;
        Dsign_final = Dsign_prev1;
    }

    /* compute binomial coefficient C(n,2) */
    seq_t binom = (seq_t)n * (seq_t)(n - 1) / (seq_t)2;

    /* sum  = d_final + (Dsign_final ? Dmag_final : -Dmag_final) */
    seq_t sum_total;
    if (Dsign_final)
        sum_total = d_final + Dmag_final;
    else
        sum_total = d_final - Dmag_final;   /* guaranteed non‑negative */

    /* a(n) = binom * (sum_total / 2) */
    seq_t result = binom * (sum_total / (seq_t)2);
    return result;
}
