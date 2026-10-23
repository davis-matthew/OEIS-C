/*
 * A128601  a(n) = the denominator of the continued fraction
 *          [1;floor(n/(n-1)),floor(n/(n-2)),...,floor(n/1)].
 * https://oeis.org/A128601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of the continued fraction * [1;
 *            floor(n/(n-1)), floor(n/(n-2)), ..., floor(n/1)]. * This is
 *            computed by iterating the convergent recurrence: * p_{-2}=0,
 *            p_{-1}=1; q_{-2}=1, q_{-1}=0 * for each term b_i: * p_i = b_i *
 *            p_{i-1} + p_{i-2} * q_i = b_i * q_{i-1} + q_{i-2} * The denom...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* sequences for convergents */
    seq_t p_prev2 = 0, p_prev1 = 1;
    seq_t q_prev2 = 1, q_prev1 = 0;
    seq_t n_seq  = (seq_t)n;

    /* iterate over the n terms: first term is 1, remaining are floor(n/(n-i)) */
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t b;
        if (i == 0) {
            b = 1;
        } else {
            b = n_seq / (seq_t)(n - i); /* floor division */
        }

        seq_t p = b * p_prev1 + p_prev2;
        seq_t q = b * q_prev1 + q_prev2;

        p_prev2 = p_prev1;
        p_prev1 = p;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }

    return q_prev1; /* denominator of the full continued fraction */
}
