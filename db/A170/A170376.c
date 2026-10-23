/*
 * A170376  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 42: a(n) = 2 + 29 * Σ_{k=0}^{n-1} a(k) For
 *            n ≥ 43: a(n) = 29 * Σ_{k=n-42}^{n-1} a(k) – 435 * a(n-43)
 *            (Derived from the rational generating function in OEIS A170376.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sliding window of the last 42 terms;
       buffer[i] holds a(i) where i = current index modulo 43. */
    seq_t buffer[43] = {0};

    seq_t window_sum = 0;   /* sum of the last 42 terms (for n ≥ 43)   */
    seq_t prefix_sum = 0;   /* sum of all terms up to the previous index  */
    seq_t term = 0;         /* a(n) being computed in the loop */

    for (idx_t i = 0; i <= n; ++i)
    {
        if (i == 0)
        {
            term = 1;                                     /* a(0) */
        }
        else if (i <= 42)
        {
            /* a(i) = 2 + 29 * Σ_{k=0}^{i-1} a(k) */
            term = 2 + 29 * prefix_sum;
        }
        else
        {
            /* a(i) = 29 * Σ_{k=i-42}^{i-1} a(k) – 435 * a(i-43) */
            idx_t out_idx = (i - 43) % 43;                 /* a(i-43) */
            term = 29 * window_sum - 435 * buffer[out_idx];
        }

        /* Store current term */
        idx_t idx = i % 43;
        buffer[idx] = term;

        /* Update running sums */
        if (i <= 42)
            prefix_sum += term;

        if (i >= 42)
        {
            idx_t old_idx = (i - 42) % 43;                 /* a(i-42) */
            seq_t old_val = buffer[old_idx];
            window_sum += term;
            window_sum -= old_val;
        }
        else
        {
            window_sum += term;
        }
    }
    return term;
}
