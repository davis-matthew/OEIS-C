/*
 * A041733  Denominators of continued fraction convergents to sqrt(386).
 * https://oeis.org/A041733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0)=1, a(1)=1.
 * For n>=2,
 *   a(n) = q_n,
 *   where q_n are the denominators of the convergents of the continued fraction for sqrt(386).  
 *   The partial quotients are: 19, 1,1,1,4,1,18,1,4,1,1,1,38, and then repeat the last 12 terms.  
 * The recurrence for the convergents is:
 *   q_-2 = 1, q_-1 = 0,
 *   q_k = a_k * q_{k-1} + q_{k-2}  (k ≥ 0),
 *   where a_0 = 19 and for k>0, a_k follows the repeating cycle
 *   {1,1,1,4,1,18,1,4,1,1,1,38}.  The result a(n) is q_n.
 */

seq_t a(idx_t n)
{
    /* The repeating cycle of partial quotients after the integer part 19 */
    static const idx_t cycle[12] = {1,1,1,4,1,18,1,4,1,1,1,38};

    /* q_-2 and q_-1 in the recurrence for convergents */
    seq_t q_prev2 = 1;   /* q_-2 */
    seq_t q_prev1 = 0;   /* q_-1 */
    seq_t q = 0;         /* current denominator */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t a_k;
        if (k == 0)
            a_k = 19;                         /* integer part of sqrt(386) */
        else
            a_k = cycle[(k - 1) % 12];         /* cycle repeats after index 1 */

        q = a_k * q_prev1 + q_prev2;           /* recurrence */
        q_prev2 = q_prev1;
        q_prev1 = q;
    }
    return q;
}
