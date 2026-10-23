/*
 * A029574  Number of permutations of an n-set containing a 7-cycle.
 * https://oeis.org/A029574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (1 - Σ_{k=0..⌊n/7⌋} (-1)^k/(k!*7^k))
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  = Σ_{k=1..⌊n/7⌋} (-1)^{k+1} * n!/(k!*7^k)  */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n < 7 no 7-cycle can appear */
    if (n < 7) {
        return (seq_t)0;
    }

    /* Maximum number of disjoint 7‑cycles */
    idx_t m = n / 7;

    /* Compute n! / 7  –  cancel one factor 7 early to keep the
       intermediate value small (for 64‑bit builds).  */
    seq_t term = (seq_t)1;
    bool div_done = false;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        if (!div_done && (i % (idx_t)7) == (idx_t)0) {
            /* first multiple of 7 – divide it out */
            term *= (seq_t)(i / (idx_t)7);
            div_done = true;
        } else {
            term *= (seq_t)i;
        }
    }

    /* term now equals n! / 7  (universal for k = 1) */
    seq_t sum = term;           /* k = 1 contributes with a positive sign */

    /* Produce remaining terms using the recurrence
       term_{k+1} = term_k / ((k+1) * 7)           */
    for (idx_t k = (idx_t)2; k <= m; ++k) {
        /* exact integer division – term_k is guaranteed divisible */
        term /= (seq_t)((k) * (idx_t)7);

        if (k & (idx_t)1) {          /* odd k → add */
            sum += term;
        } else {                     /* even k → subtract */
            sum -= term;
        }
    }

    return sum;
}
