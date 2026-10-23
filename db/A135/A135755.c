/*
 * A135755  a(n) = Sum_{k=0..n} C(n,k)*3^[k*(k-1)/2].
 * https://oeis.org/A135755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)*3^[k*(k-1)/2]
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* iterative computation:
       * c   : current binomial coefficient C(n,k)
       * p   : current value 3^[k*(k-1)/2]
       * t   : 3^(k-1) (used to update p)
       * sum : accumulated result
       */
    seq_t sum = 0;
    seq_t c   = 1;   /* C(n,0) */
    seq_t p   = 1;   /* 3^0 */
    seq_t t   = 1;   /* 3^0, will become 3^(k-1) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Add current term */
        sum += c * p;

        if (k == n) /* no need to update for last iteration */
            break;

        /* Update binomial coefficient: C(n,k+1) = C(n,k) * (n-k) / (k+1) */
        c = (c * (seq_t)(n - k)) / (seq_t)(k + 1);

        /* Update exponent part:
           p_{k+1} = p_k * 3^k
           t holds 3^k after this step
        */
        p = p * t;
        t = t * (seq_t)3;
    }

    return sum;
}
