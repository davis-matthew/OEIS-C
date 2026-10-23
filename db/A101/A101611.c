/*
 * A101611  a(n) = n! * Sum_{k=ceiling(n/2)..n} 1/k.
 * https://oeis.org/A101611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=ceil(n/2)..n} 1/k Recurrence used: a(1) = 1
 *            for n even: a(n) = n * a(n-1) + (n-1)! for n odd: a(n) = n *
 *            a(n-1) - (n+1) * (n-2)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)      /* undefined behaviour for n < 1; return 0 */
        return 0;

    /* factorials for (i-1)! and (i-2)! */
    seq_t fact_prev2 = 1;   /* (0)! */
    seq_t fact_prev  = 1;   /* (1)! */

    seq_t a_prev = 1;       /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t a_i;
        seq_t i_seq = (seq_t)i;

        if ((i & 1) == 0) {   /* even i */
            a_i = i_seq * a_prev + fact_prev;
        } else {              /* odd i */
            a_i = i_seq * a_prev - ((i_seq + 1) * fact_prev2);
        }

        /* update factorials for next iteration */
        seq_t fact_new = fact_prev * i_seq;  /* i!   */
        fact_prev2 = fact_prev;              /* (i-1)! */
        fact_prev  = fact_new;               /* i!     */

        a_prev = a_i;
    }

    return a_prev;
}
