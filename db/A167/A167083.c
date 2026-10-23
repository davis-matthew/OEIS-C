/*
 * A167083  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A167083 follows a linear recurrence derived from its generating
 *            function. For n ≥ 0 let p(n) = 1 if n = 0 or 13, p(n) = 2 for 1
 *            ≤ n ≤ 12, and p(n) = 0 otherwise. Then a(n) = p(n) +
 *            28·(a(n−1)+⋯+a(n−12)) – 406·a(n−13), with a(k) = 0 for k < 0.
 *            This uses the denominator 406·t¹³ – 28·t¹² – … – 28·t...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Circular buffer storing the last 13 computed terms. */
    seq_t buf[13] = {0};

    for (idx_t i = 0; i <= n; ++i)
    {
        /* Sum of a(i-1) … a(i-12) (zeros for negative indices). */
        seq_t sum12 = 0;
        for (idx_t k = 1; k <= 12; ++k)
        {
            if (i >= k)
                sum12 += buf[(i - k) % 13];
        }

        /* a(i-13) (zero for i < 13). */
        seq_t a_13 = 0;
        if (i >= 13)
            a_13 = buf[(i - 13) % 13];

        /* Value of p(i). */
        seq_t pi = 0;
        if (i == 0 || i == 13)
            pi = 1;
        else if (i >= 1 && i <= 12)
            pi = 2;

        /* a(i) = pi + 28*sum12 – 406*a_13. */
        seq_t ai = pi + (seq_t)28 * sum12 - (seq_t)406 * a_13;

        buf[i % 13] = ai;

        if (i == n)
            return ai;
    }

    /* Should never reach here, but keep the compiler happy. */
    return buf[n % 13];
}
