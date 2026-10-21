/*
 * A238740  a(n) = (n+2)!^2*(n+1)!/4*hypergeom([-n],[2,3,3],-1).
 * https://oeis.org/A238740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2)!^2*(n+1)!/4*hypergeom([-n],[2,3,3],-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 19, a(2) = 961, ... (OEIS A238740)          */

#include <stdbool.h>

/* Using the helper factorial(idx_t) from ../utils/factorial.h */
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Compute P = (n+2)!^2 * (n+1)! / 4 as an integer. */
    seq_t fact_np1 = factorial(n + 1);      /* (n+1)! */
    seq_t fact_np2 = factorial(n + 2);      /* (n+2)! */
    seq_t P = fact_np2 * fact_np2 * fact_np1; /* (n+2)!^2 * (n+1)! */
    P /= (seq_t)4;                          /* exact division by 4 */

    seq_t term = P;        /* t0 */
    seq_t sum  = term;     /* a(0) = P, corresponds to k = 0 */

    /* Recurrence for subsequent terms:
       t_k = t_{k-1} * (n-k+1) / [k*(k+1)*(k+2)^2]   for k >= 1   */
    for (idx_t k = 1; k <= n; ++k) {
        /* numerator: t_{k-1} * (n-k+1)  */
        seq_t num = term * (seq_t)(n - k + 1);

        /* denominator: k*(k+1)*(k+2)^2 */
        seq_t denom = (seq_t)k;
        denom *= (k + 1);
        denom *= (k + 2);
        denom *= (k + 2);

        term = num / denom;   /* guaranteed exact division */
        sum += term;
    }

    return sum;
}
