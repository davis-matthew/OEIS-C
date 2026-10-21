/*
 * A088920  Solutions k to the Diophantine equation k = 2n^2 = m^2+1.
 * https://oeis.org/A088920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*y_n^2 where y_1 = 1, y_2 = 5, y_{k+1} = 6*y_k - y_{k-1}
 *            (k ≥ 2) The sequence {y_k} gives the positive solutions of x^2 –
 *            2y^2 = -1 (negative Pell equation), so a(n) = 2*y_n^2 is the
 *            n‑th term of A088920
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1 – behaviour below is irrelevant */
        return 0;
    if (n == 1)            /* y_1 = 1 → 2*1^2 = 2 */
        return (seq_t)2;
    /* start with y_1 = 1, y_2 = 5 */
    seq_t y_prev = 1;
    seq_t y_curr = 5;
    if (n == 2)            /* 2*5^2 = 50 */
        return (seq_t)50;
    for (idx_t i = 3; i <= n; ++i)
    {
        /* y_next = 6*y_curr - y_prev    (fits in seq_t) */
        seq_t y_next = (seq_t)((seq_t)6 * y_curr - y_prev);
        y_prev = y_curr;
        y_curr = y_next;
    }
    /* return 2*y_curr^2 */
    seq_t k = y_curr * y_curr;
    return (seq_t)2 * k;
}
