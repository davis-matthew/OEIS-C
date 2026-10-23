/*
 * A169245  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A169245
   a(0) = 1
   For 1 ≤ n < 28 :  a(n) = p_n + 38 · Σ_{k=0}^{n-1} a(k)      (p_n = 2 for n ≥ 1, p_0 = 1)
   For n ≥ 28      :  a(n) = 38 · Σ_{k=n-27}^{n-1} a(k) – 741 · a(n-28)
   (The coefficients come from the rational generating function
    (1 + 2t + … + 2t^27 + t^28) / (1 – 38t – … – 38t^27 + 741t^28).) */
seq_t a(idx_t n)
{
    /* compute the first 28 terms in an array  */
    seq_t base[28];
    seq_t prefix = (seq_t)0;
    for (idx_t i = (idx_t)0; i <= (idx_t)27; ++i)
    {
        seq_t p = (i == (idx_t)0) ? (seq_t)1 : (seq_t)2;
        seq_t val = p + (seq_t)38 * prefix;
        base[i] = val;
        prefix += val;
    }
    if (n <= (idx_t)27)
        return base[n];

    /* initialise sliding window of last 27 terms and a ring buffer */
    seq_t buf[28];
    for (idx_t i = (idx_t)0; i <= (idx_t)27; ++i)
        buf[i] = base[i];

    /* sum of last 27 terms: a[1] + … + a[27] */
    seq_t sum27 = (seq_t)0;
    for (idx_t i = (idx_t)1; i <= (idx_t)27; ++i)
        sum27 += buf[i];

    seq_t val;
    for (idx_t i = (idx_t)28; i <= n; ++i)
    {
        /* recurrence for n ≥ 28 */
        val = (seq_t)38 * sum27 - (seq_t)741 * buf[(i - (idx_t)28) % (idx_t)28];

        /* store new value */
        buf[i % (idx_t)28] = val;

        if (i == n)
            return val;

        /* update sum27 for next step: add new term, subtract term leaving window */
        seq_t outgoing = buf[(i - (idx_t)27) % (idx_t)28];
        sum27 += val;
        sum27 -= outgoing;
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
