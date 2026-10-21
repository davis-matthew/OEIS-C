/*
 * A077232  a(n) is smallest natural number a satisfying Pell equation a^2 -
 *          d(n)*b^2= +1 or = -1, with d(n)=A000037(n) (a nonsquare).
 *          Corresponding smallest b(n)=A077233(n).
 * https://oeis.org/A077232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 84, u32 n <= 184, u64 n <= 503,
 *            u128 n <= 1684, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A077232:  a(n) = smallest positive integer a solving
 *                 a^2 - d(n) * b^2 = ±1,
 *                 where d(n) is the n‑th nonsquare integer
 *                 (A000037).  The sign is chosen to give the
 *                 smaller a.  This implementation uses the
 *                 continued‑fraction method for the minimal
 *                 Pell solution of each nonsquare d(n). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* ----------------------------------------------------------------------
 *  Return the n‑th positive nonsquare integer (n >= 1).
 *  Uses a binary search on the monotone function f(x)=x-floor_sqrt(x)
 *  which counts the nonsquares up to x.
 * ---------------------------------------------------------------------- */
static uint64_t non_square_n(idx_t n)
{
    idx_t low  = n;                                 /* x >= n            */
    idx_t high = n + (idx_t)floor_sqrt((seq_t)n) + 20; /* safe upper bound */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t mid_s = (seq_t)mid;
        idx_t ns = (idx_t)(mid_s - floor_sqrt(mid_s)); /* nonsquares <= mid */

        if (ns < n) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return (uint64_t)low;     /* D fits in 64 bits for the tested ranges */
}

/* ----------------------------------------------------------------------
 *  Compute the minimal solution a of the Pell equation
 *  a^2 - D·b^2 = ±1 for a given nonsquare D (D >= 2).
 *  The algorithm follows the standard continued‑fraction
 *  technique: the fundamental solution is given by the
 *  convergent at index L‑1, where L is the period length
 *  of √D.  That solution satisfies
 *     type(D) = +1 if L is even, -1 if L is odd.
 *  For our sequence we need only that minimal a.
 * ---------------------------------------------------------------------- */
static seq_t minimal_pell_a(uint64_t D)
{
    seq_t sqrt_D = floor_sqrt((seq_t)D);
    idx_t a0 = (idx_t)sqrt_D;                 /* integer part of √D */

    /*  Convergents:  p_{-2}=0, p_{-1}=1;  q_{-2}=1, q_{-1}=0  */
    seq_t prev2_p = 0, prev1_p = 1;
    seq_t prev2_q = 1, prev1_q = 0;

    /* first convergent using a0  (index 0) */
    seq_t p = (seq_t)a0 * prev1_p + prev2_p;  /* = a0 */
    seq_t q = (seq_t)a0 * prev1_q + prev2_q;  /* = 1     */
    prev2_p = prev1_p;  prev1_p = p;
    prev2_q = prev1_q;  prev1_q = q;

    /* variables for the CF iteration */
    idx_t m = 0;          /* m_i */
    idx_t d_curr = 1;     /* d_i */
    idx_t a_curr = a0;    /* a_i */

    while (true) {
        /* next CF coefficients */
        idx_t m_next = d_curr * a_curr - m;
        idx_t d_next = (D - (idx_t)((seq_t)m_next * (seq_t)m_next)) / d_curr;
        idx_t a_next = (a0 + m_next) / d_next;

        /* period ends when a_next == 2*a0 */
        if (a_next == 2 * a0) {
            break;
        }

        /* compute next convergent */
        seq_t new_p = (seq_t)a_next * prev1_p + prev2_p;
        seq_t new_q = (seq_t)a_next * prev1_q + prev2_q;
        prev2_p = prev1_p;  prev1_p = new_p;
        prev2_q = prev1_q;  prev1_q = new_q;

        /* advance CF state */
        m      = m_next;
        d_curr = d_next;
        a_curr = a_next;
    }

    /* prev1_p now holds the minimal a at index L‑1 */
    return prev1_p;
}

/* ----------------------------------------------------------------------
 *  Public sequence function required by the framework.
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* undefined for n <= 0 */
    uint64_t D = non_square_n(n);              /* d(n) = nth nonsquare */
    return minimal_pell_a(D);
}
