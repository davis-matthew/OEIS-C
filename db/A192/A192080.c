/*
 * A192080  Expansion of 1/((1-x)^6 - x^6).
 * https://oeis.org/A192080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A192080: Expansion of 1/((1-x)^6 - x^6)
   G.f.: 1/((1-2x)*(1-x+x^2)*(1-3x+3x^2))
   Recurrence: a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5)
   Initial values: a(0)=1, a(1)=6, a(2)=21, a(3)=56, a(4)=126
   Valid for all n ≥ 0.  */
#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Handle known initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)21;
    if (n == 3) return (seq_t)56;
    if (n == 4) return (seq_t)126;

    /* Non‑recursive variables to keep the last 5 terms */
    seq_t a0 = (seq_t)1,   /* a(n-5)   */
           a1 = (seq_t)6,   /* a(n-4)   */
           a2 = (seq_t)21,  /* a(n-3)   */
           a3 = (seq_t)56,  /* a(n-2)   */
           a4 = (seq_t)126; /* a(n-1)   */
    seq_t ai;

    /* Iterate up to n, updating the sliding window */
    for (idx_t i = 5; i <= n; ++i) {
        ai = (seq_t)6 * a4
            - (seq_t)15 * a3
            + (seq_t)20 * a2
            - (seq_t)15 * a1
            + (seq_t)6  * a0;

        /* shift terms for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }

    return a4;
}
