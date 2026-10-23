/*
 * A127615  a(n) = denominator of the continued fraction which has the
 *          positive integers which are <= n and are coprime to n as its
 *          terms. The terms are written in order from 1 for the integer part,
 *          to n-1 for the final term of the continued fraction.
 * https://oeis.org/A127615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of the continued fraction which has the
 *            positive integers <= n that are coprime to n as its terms. The
 *            continued fraction is 1 + 1/(3 + 1/(5 + 1/7)) for n = 8, and the
 *            function returns the denominator of that convergent
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 22, u128 n <= 25,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is obtained by the standard recurrence for continued
   fractions:
        p_{-2} = 0,  p_{-1} = 1
        q_{-2} = 1,  q_{-1} = 0
        p   = a·p_{-1} + p_{-2}
        q   = a·q_{-1} + q_{-2}
   where a runs over the coprime numbers in ascending order.  The
   function computes the result on demand; no memoisation is used
   because O(n) time is trivial for the supported input range. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1 */

    if (n == 1) {
        return (seq_t)1;
    }

    /* Initial values for the recurrence */
    seq_t p_prev2 = (seq_t)0;   /* p_{-2} */
    seq_t p_prev1 = (seq_t)1;   /* p_{-1} */
    seq_t q_prev2 = (seq_t)1;   /* q_{-2} */
    seq_t q_prev1 = (seq_t)0;   /* q_{-1} */

    for (idx_t k = 1; k < n; ++k) {
        /* Only use numbers coprime to n */
        if (gcd((seq_t)k, (seq_t)n) == (seq_t)1) {
            seq_t a_term = (seq_t)k;

            seq_t p = a_term * p_prev1 + p_prev2;
            seq_t q = a_term * q_prev1 + q_prev2;

            p_prev2 = p_prev1;
            p_prev1 = p;
            q_prev2 = q_prev1;
            q_prev1 = q;
        }
    }

    return q_prev1;
}
