/*
 * A081295  a(n) = (-1)^(n+1) * coefficient of x^n in Sum_{k>=1}
 *          x^k/(1+2*x^k).
 * https://oeis.org/A081295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^(n+1) * Σ_{d|n} (-2)^(d-1) (offset: a(1) is the
 *            first term)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes a(n) for n ≥ 1 in O( √n ) time.
   Uses a cached array of powers of two to avoid recomputing them. */
seq_t a(idx_t n)
{
    /* n < 1 is outside the official offset – return 0 for safety */
    if (n == 0) return (seq_t)0;

    /* --------------------------------------------------------------------
       Cached powers of two: pow2[e] = 2^e,  for e ≥ 0.
       The array is grown lazily on the first call that requires a new
       exponent.  All operations are performed with the type seq_t,
       which may be an unsigned integer or an arbitrary‑precision class.
       -------------------------------------------------------------------- */
    static seq_t *pow2 = NULL;   /* dynamic array of powers of two           */
    static idx_t pow2_size = 0;  /* number of elements currently allocated   */

    /* We need powers up to exponent n‑1 inclusive, so allocate size n+1. */
    idx_t needed = n;                      /* indices 0 … needed-1 are required      */
    if (pow2_size <= needed) {
        idx_t new_size = needed + 1;       /* allocate 0 … needed */
        seq_t *tmp = (seq_t *)realloc(pow2, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0;        /* allocation failure – return 0           */
        /* initialise the new entries                     */
        for (idx_t i = pow2_size; i < new_size; ++i) {
            if (i == 0) {
                tmp[i] = (seq_t)1;
            } else {
                /* pow2[i] = pow2[i-1] * 2 */
                tmp[i] = tmp[i-1] * (seq_t)2;
            }
        }
        pow2 = tmp;
        pow2_size = new_size;
    }

    /* --------------------------------------------------------------------
       Accumulate contributions from each divisor d of n.
       sum_pos   holds the sum of 2^(d-1) for divisors where (d-1) is even.
       sum_neg   holds the sum of 2^(d-1) for divisors where (d-1) is odd.
       -------------------------------------------------------------------- */
    seq_t sum_pos = (seq_t)0;
    seq_t sum_neg = (seq_t)0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        /* divisor d ----------------------------------------------------- */
        seq_t term = pow2[d-1];               /* 2^(d-1) */
        if ((d-1) & 1) {                      /* odd exponent → negative sign */
            sum_neg += term;
        } else {
            sum_pos += term;
        }

        idx_t e = n / d;                       /* complementary divisor   */
        if (e != d) {
            term = pow2[e-1];
            if ((e-1) & 1) {
                sum_neg += term;
            } else {
                sum_pos += term;
            }
        }
    }

    /* --------------------------------------------------------------------
       Apply the outer sign (-1)^(n+1).  For odd n the sign is +1; for
       even n the sign is –1.  Because the final sequence values are
       always positive we can avoid negative intermediate values by using
       the same two accumulators: for even n we simply swap the roles.
       -------------------------------------------------------------------- */
    seq_t result;
    if (n & 1) {               /* n odd → sign +1  */
        result = sum_pos - sum_neg;   /* = Σ (-2)^(d-1)  */
    } else {                   /* n even → sign –1 */
        result = sum_neg - sum_pos;   /* = - Σ (-2)^(d-1) */
    }

    return result;
}
