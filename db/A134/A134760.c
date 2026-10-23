/*
 * A134760  a(n) = 2*A000984(n) - 1.
 * https://oeis.org/A134760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * C(2n,n) – 1; C(2n,n) is the central binomial
 *            coefficient
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"

/* Compute the nth term of OEIS A134760.  Works for any build where
   seq_t and idx_t are integer types or an arbitrary‑precision integer
   class.  No dynamic memory allocation, no external helpers, and no
   floating point. */
seq_t a(idx_t n)
{
    /* C(2n,n) is computed multiplicatively with exact integer arithmetic. */
    seq_t c = 1;           /* Current binomial coefficient. */
    seq_t n_seq = (seq_t)n;/* Cache of n as seq_t to avoid repeated casts. */

    for (idx_t i = 1; i <= n; ++i) {
        /* Intermediate values for this step. */
        seq_t a_term = n_seq + (seq_t)i;   /* numerator: n + i          */
        seq_t b_term = (seq_t)i;           /* denominator: i            */

        /* First cancel common factors between a_term and b_term. */
        seq_t g = gcd(a_term, b_term);
        a_term /= g;
        b_term /= g;

        /* Then cancel remaining common factors with the current binomial
           coefficient c.  After both reductions b_term must become 1,
           guaranteeing exact division. */
        g = gcd(c, b_term);
        c /= g;
        b_term /= g;

        /* b_term should be 1 now; multiply to update c. */
        c = c * a_term;
    }

    /* Return 2*C(2n,n) - 1. */
    return c * 2 - 1;
}
