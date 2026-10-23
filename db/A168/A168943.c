/*
 * A168943  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 26, a(2) = 650, For n ≥ 3, a(n) is computed by
 *            the recurrence a(n) = 25 · a(n−1) – f(n−1,21), where f(k,r) is
 *            the number of reduced words of length k that end with a run of r
 *            alternating letters (2 ≤ r ≤ 21). The recurrence for f is:
 *            f(k,2) = 24 · Σ_{i=2}^{21} f(k−1,i) f(k,r) = f...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A168943. */
seq_t a(idx_t n)
{
    /* Handle the offset 0, 1, 2 explicitly. */
    if (n == 0) return 1;          /* a(0) = 1 */
    if (n == 1) return 26;         /* a(1) = 26 */
    if (n == 2) return 650;        /* a(2) = 650 */

    /* f[2..21] holds counts for the current length. */
    seq_t f[22] = {0};
    f[2] = 650;                    /* base for length 2 */

    for (idx_t len = 3; len <= n; ++len)
    {
        /* Compute the sum Σ_{i=2}^{21} f[i] for the previous length. */
        seq_t sumPrev = 0;
        for (int r = 2; r <= 21; ++r)
            sumPrev += f[r];

        /* Update the array for the new length.
         * f[2] = 24 · sumPrev
         * f[r] = old f[r-1] for r = 3..21
         */
        seq_t newf[22] = {0};
        newf[2] = 24 * sumPrev;
        for (int r = 3; r <= 21; ++r)
            newf[r] = f[r - 1];

        /* Copy the new values back into f. */
        for (int r = 2; r <= 21; ++r)
            f[r] = newf[r];
    }

    /* Sum all f[2..21] to obtain a(n). */
    seq_t result = 0;
    for (int r = 2; r <= 21; ++r)
        result += f[r];

    return result;
}
