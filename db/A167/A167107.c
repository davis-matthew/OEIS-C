/*
 * A167107  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A167107:
   a(0) = 1
   a(n) = 6*5^(n-1)           for 1 ≤ n ≤ 13
   a(14) = 4*Σ_{k=1}^{13} a(14-k) – 10*a(0) + 1
   a(n) = 4*Σ_{k=1}^{13} a(n-k) – 10*a(n-14)   for n > 14
*/
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)1;

    /* compute the first 14 terms (indices 0 … 13) */
    seq_t first[14];
    first[0] = (seq_t)1;
    seq_t v = (seq_t)6;                /* 6*5^(i-1) */
    for (idx_t i = 1; i < 14; ++i) {
        first[i] = v;
        v *= (seq_t)5;
    }
    if (n < 14)
        return first[n];

    /* sliding window holding the last 14 computed values */
    seq_t last[14];
    for (idx_t i = 0; i < 14; ++i)
        last[i] = first[i];

    /* constants used in the recurrence */
    const seq_t four = (seq_t)4;
    const seq_t ten  = (seq_t)10;
    const seq_t one  = (seq_t)1;
    seq_t cur = 0;

    for (idx_t i = 14; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < 14; ++k)
            sum += last[k];

        if (i == 14)
            cur = four * sum - ten * last[0] + one;
        else
            cur = four * sum - ten * last[0];

        /* shift the sliding window for the next iteration */
        for (idx_t k = 0; k < 13; ++k)
            last[k] = last[k + 1];
        last[13] = cur;
    }

    return cur;
}
