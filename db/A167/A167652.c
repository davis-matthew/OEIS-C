/*
 * A167652  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n) + 5 * Σ_{k=1}^{14} a(n-k) – 15 * a(n-15), where
 *            p(0)=1, p(1…14)=2, p(15)=1, p(n>15)=0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    /* 15‑term ring buffer holding the last values a(n‑15)…a(n-1). */
    seq_t last[15] = { 0 };
    last[0] = (seq_t)1;          /* a(0) */

    seq_t term;
    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        /* numerator coefficient p(i) */
        seq_t p;
        if (i == (idx_t)15)
            p = (seq_t)1;
        else if (i <= (idx_t)14)
            p = (seq_t)2;
        else
            p = (seq_t)0;

        term = p;

        /* add 5 * a(i-k) for k = 1..14 */
        for (int k = 1; k <= 14; ++k)
        {
            if (i >= (idx_t)k)
            {
                idx_t idx = (i - (idx_t)k) % (idx_t)15;
                term += ((seq_t)5) * last[idx];
            }
        }

        /* subtract 15 * a(i-15) when i ≥ 15 */
        if (i >= (idx_t)15)
        {
            idx_t idx = (i - (idx_t)15) % (idx_t)15;
            term -= ((seq_t)15) * last[idx];
        }

        last[i % (idx_t)15] = term;
    }

    return last[n % (idx_t)15];
}
