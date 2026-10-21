/*
 * A061714  Number of types of (n-1)-swap moves for traveling salesman
 *          problem. Number of circular permutations on elements 0,1,...,2n-1
 *          where every two elements 2i,2i+1 and no two elements 2i-1,2i are
 *          adjacent.
 * https://oeis.org/A061714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (-1)^n + Σ_{i=0}^{n-1} (-1)^(n-1-i) * C(n,i+1) * i! * 2^i
 *            This implementation computes the sum term‑by‑term, avoiding any
 *            intermediate overflows or large cached tables. The algorithm is
 *            O(n) for each call and works for any build of seq_t (fixed‑width
 *            or arbitrary‑precision)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* special case n = 0 */
    if (n == 0u)
        return (seq_t)1u;

    seq_t sum_pos = (seq_t)0u;
    seq_t sum_neg = (seq_t)0u;

    /* initialize the first terms of the recurrence */
    seq_t fact  = (seq_t)1u;          /* i!  when i = 0  */
    seq_t pow2  = (seq_t)1u;          /* 2^i when i = 0  */
    seq_t C     = (seq_t)n;           /* C(n,1) */

    for (idx_t i = 0u; i < n; ++i)
    {
        /* compute the current term */
        seq_t term = C * fact;
        term = term * pow2;

        /* add to the appropriate sum */
        if (((n - 1u - i) & 1u) == 0u)   /* sign = +1 */
            sum_pos += term;
        else
            sum_neg += term;

        /* stop after the last term (i == n-1) */
        if (i == n - 1u)
            break;

        /* advance fact = (i+1)! for the next iteration */
        fact *= (seq_t)(i + 1u + 0u);   /* i+1 */

        /* advance pow2 = 2^(i+1) */
        pow2 *= (seq_t)2u;

        /* update binomial coefficient:
           C = C * (n-i-1) / (i+2)   (C holds C(n,i+1) before the update)
        */
        seq_t numerator = C * (seq_t)(n - (i + 1u));
        C = numerator / (seq_t)(i + 2u);
    }

    /* sum_pos - sum_neg is guaranteed non‑negative for all n >= 0 */
    seq_t diff = sum_pos - sum_neg;

    /* add the (-1)^n term */
    if ((n & 1u) == 0u)    /* n even: (+1) */
        return diff + (seq_t)1u;
    else                   /* n odd:  (-1) */
        return diff - (seq_t)1u;
}
