/*
 * A042520  Numerators of continued fraction convergents to sqrt(789).
 * https://oeis.org/A042520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A042520: Numerators of continued fraction convergents to √789
 *
 * The sequence is given by the recurrence on the convergents of
 * the continued fraction of √789.  Let a0 = ⌊√789⌋ = 28.  For
 * n ≥ 1 let a_n be the nth partial quotient of the continued
 * fraction.  The numerators p_n of the convergents satisfy:
 *
 *        p_{-2} = 0,  p_{-1} = 1
 *        p_n   = a_n * p_{n-1} + p_{n-2}   (n ≥ 0)
 *
 * For √789 the periodic part of the continued fraction, after
 * a0, is
 *
 *   {11,4,1,1,2,3,1,13,3,1,2,18,2,1,3,13,1,3,2,1,1,4,11,56}
 *
 * which has length 24.  The code below uses this pre‑computed
 * period and evaluates the recurrence iteratively for any n ≥ 0.
 * It works for all seq_t types: unsigned integers of any size
 * or an arbitrary‑precision integer class (compiled as C++17).
 */

#include <stdint.h>

/* Periodic partial quotients of √789 after a0 = 28.  */
static const seq_t PERIOD[] = {
    11, 4, 1, 1, 2, 3, 1, 13, 3, 1,
    2, 18, 2, 1, 3, 13, 1, 3, 2, 1,
    1, 4, 11, 56
};
static const idx_t PERIOD_LEN = sizeof(PERIOD) / sizeof(PERIOD[0]);

seq_t a(idx_t n)
{
    /* First partial quotient equals ⌊√789⌋ = 28.  */
    const seq_t A0 = 28;

    /* Convergents recurrence variables.  */
    seq_t p_prev2 = 0;  /* p_{-2} */
    seq_t p_prev1 = 1;  /* p_{-1} */
    seq_t p_curr  = 0;  /* p_n  (will hold the result) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t a_k;
        if (k == 0) {
            a_k = A0;
        } else {
            /* Cycle through the periodic part.  */
            a_k = PERIOD[(k - 1) % PERIOD_LEN];
        }
        p_curr = a_k * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }
    return p_curr;
}
