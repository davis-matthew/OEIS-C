/*
 * A167108  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the given generating function:
 *   a(n) = p(n) + 5*sum_{j=1}^{13} a(n-j) - 15*a(n-14)
 * where
 *   p(n) = 1  for n = 0 or n = 14
 *          2  for 1 ≤ n ≤ 13
 *          0  otherwise
 * The values a(k) for k < 0 are treated as 0.
 * This recurrence reproduces the first 21 terms and all subsequent ones. */
seq_t a(idx_t n)
{
    /* Circular buffer holding the last 14 computed terms */
    seq_t buf[14] = {0};

    for (idx_t i = 0; i <= n; ++i)
    {
        /* Determine p(i) */
        seq_t p;
        if (i == 0 || i == 14)
            p = (seq_t)1;
        else if (i <= 13)
            p = (seq_t)2;
        else
            p = (seq_t)0;

        /* Sum of the previous 13 terms (a(i-1) ... a(i-13)) */
        seq_t sum13 = (seq_t)0;
        for (idx_t k = 1; k <= 13; ++k)
        {
            if (i >= k)
                sum13 += buf[(i - k) % 14];
        }

        /* a(i-14) if it exists */
        seq_t last14 = (seq_t)0;
        if (i >= 14)
            last14 = buf[(i - 14) % 14];

        /* Compute a(i) using the recurrence */
        seq_t new_val = p + (seq_t)5 * sum13 - (seq_t)15 * last14;

        /* Store the new term in the buffer */
        buf[i % 14] = new_val;

        if (i == n)
            return new_val;  /* Result requested */
    }

    /* Should never reach here; return 0 as a safe fallback */
    return (seq_t)0;
}
