/*
 * A165786  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence derived from the generating function of A165786.
 *   a(0) = 1
 *   for n ≥ 1:
 *       a(n) = N(n) + Σ_{i=1}^{9} 6·a(n‑i) – 21·a(n‑10)
 *   where N(0) = 1, N(1)…N(9) = 2, N(10) = 1, N(n) = 0 for n > 10
 *
 * This recurrence reproduces the sequence whose first terms are
 * 1, 8, 56, 392, … as listed in OEIS A165786.
 */

seq_t a(idx_t n)
{
    /* buffer holding the last 10 computed values a(k) for k = n-9 … n */
    seq_t buf[10] = {0};

    for (idx_t k = 0; k <= n; ++k)
    {
        /* numerator coefficient N(k) */
        seq_t cur;
        if (k == 0)
            cur = (seq_t)1;
        else if (k <= 9)
            cur = (seq_t)2;
        else if (k == 10)
            cur = (seq_t)1;
        else
            cur = (seq_t)0;

        /* add 6·a(k‑i) for i = 1 … 9, if available */
        if (k >= 1) cur += buf[(k - 1) % 10] * (seq_t)6;
        if (k >= 2) cur += buf[(k - 2) % 10] * (seq_t)6;
        if (k >= 3) cur += buf[(k - 3) % 10] * (seq_t)6;
        if (k >= 4) cur += buf[(k - 4) % 10] * (seq_t)6;
        if (k >= 5) cur += buf[(k - 5) % 10] * (seq_t)6;
        if (k >= 6) cur += buf[(k - 6) % 10] * (seq_t)6;
        if (k >= 7) cur += buf[(k - 7) % 10] * (seq_t)6;
        if (k >= 8) cur += buf[(k - 8) % 10] * (seq_t)6;
        if (k >= 9) cur += buf[(k - 9) % 10] * (seq_t)6;

        /* subtract 21·a(k‑10), if available */
        if (k >= 10) cur -= buf[(k - 10) % 10] * (seq_t)21;

        /* store the result for future use */
        buf[k % 10] = cur;
    }

    return buf[n % 10];
}
