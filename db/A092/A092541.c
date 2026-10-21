/*
 * A092541  Minimal values of m=a^2+b^2=c^2+d^2 for each x=a+b+c+d (a,b,c,d
 *          positive integers).
 * https://oeis.org/A092541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   _floor_isqrt_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A092541 – Minimal value of m = a² + b² = c² + d²
    where a, b, c, d are positive integers and a+b+c+d = x.
    Sequence offset: 1  (a(1) corresponds to the smallest even x ≥ 18
    for which a representation exists – i.e. x = 2·X with X composite).
    
    Formula (derived in the OEIS entry):
        • Let X be the n‑th composite integer ≥ 9.
        • Set x = 2·X.
        • Find the largest divisor t of X with t ≤ floor(√X) such that
          (t is odd or x/(2t) is odd)   OR   (t is even and x/(4t) is even).
        • If the first condition holds,  a(n) = ((t²+1)*((x/(2t))²+1))/2
          otherwise (second condition holds) a(n) = 2*(t²+1)*((x/(4t))²+1).
*/

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/_floor_isqrt_u64.h"

/* Return the n-th term of A092541. */
seq_t a(idx_t n)
{
    /* Find the n-th composite X ≥ 9. */
    idx_t X = 9;          /* 9 is composite */
    idx_t count = 1;      /* position of X in the composite list */
    while (count < n) {
        ++X;
        if (!is_prime(X))
            ++count;
    }

    /* x = 2·X. */
    seq_t x = (seq_t)X * (seq_t)2;

    /* Largest divisor t of X with t ≤ sqrt(X) satisfying a parity condition. */
    idx_t sqrt_X  = _floor_isqrt_u64(X);
    idx_t chosen_t = 0;          /* divisor t */
    for (idx_t d = sqrt_X; d >= 1; --d) {
        if (X % d != 0)
            continue;

        idx_t t            = d;               /* divisor we are testing   */
        idx_t q            = X / t;           /* complementary divisor    */
        seq_t x_div_2t     = x / ((seq_t)2 * (seq_t)t);   /* x/(2t)   */
        seq_t x_div_4t     = x / ((seq_t)4 * (seq_t)t);   /* x/(4t)   */

        /* First condition: t odd or x/(2t) odd. */
        bool first_cond  = ((t & 1U) != 0U) || ((x_div_2t & 1U) != 0U);

        /* Second condition: t even and x/(4t) even. */
        bool second_cond = ((t & 1U) == 0U) && ((x_div_4t & 1U) == 0U);

        if (first_cond || second_cond) {
            chosen_t = t;
            /* Use the appropriate formula. */
            seq_t t_sq_plus1  = (seq_t)t * (seq_t)t + (seq_t)1;
            seq_t result;
            if (first_cond) {
                seq_t d_sq = x_div_2t * x_div_2t + (seq_t)1;
                result = t_sq_plus1 * d_sq;
                /* Division by 2 is guaranteed to be integer. */
                result = result / (seq_t)2;
            } else { /* second_cond */
                seq_t e_sq = x_div_4t * x_div_4t + (seq_t)1;
                result = (seq_t)2 * t_sq_plus1 * e_sq;
            }
            return result;
        }
    }

    /* In principle we should never reach here because a composite X
       always yields a valid representation.  Return 0 as a fallback. */
    return (seq_t)0;
}
