/*
 * A168822  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function
   G(t) = (t^19 + 2*t^18 + ... + 2*t + 1) /
          (1128*t^19 - 47*t^18 - ... - 47*t + 1)

   For n >= 19   a(n) = 47*a(n-1)+…+47*a(n-18) - 1128*a(n-19)

   For 0 <= n < 19 the coefficients of the denominator give
   a(n) = p_n - Σ_{j=1}^{n} q_j * a(n-j)
   where p_0 = 1, p_1 … p_18 = 2, p_19 = 1, and
   q_j = -47 (1 ≤ j ≤ 18), q_19 = 1128.
   These initial conditions produce the OEIS sequence A168822.   */

seq_t a(idx_t n)
{
    /* a circular buffer holding the last 19 values a[k-19 .. k-1] */
    seq_t buf[19];
    idx_t i, k;
    /* initialise buffer entries to zero */
    for (i = 0; i < 19; ++i)
        buf[i] = 0;

    for (k = 0; k <= n; ++k)
    {
        seq_t sum;

        /* coefficient p_k from the numerator */
        if (k == 0)
            sum = 1;
        else if (k <= 18)
            sum = 2;
        else if (k == 19)
            sum = 1;
        else
            sum = 0;

        /* subtract/ add contributions from the denominator */
        for (i = 1; i <= 19 && i <= k; ++i)
        {
            /* index of a[k-i] in the circular buffer */
            idx_t idx = (k - i) % 19;
            seq_t prev = buf[idx];

            if (i <= 18)            /* q_i = -47  →  +47*prev */
                sum += ((seq_t)47) * prev;
            else                    /* i == 19, q_19 = 1128 →  -1128*prev */
                sum -= ((seq_t)1128) * prev;
        }

        /* store the computed a(k) */
        buf[k % 19] = sum;

        if (k == n)
            return sum;
    }
    /* unreachable, but required for completeness */
    return 0;
}
