/*
 * A169375  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^31 = I.
 * https://oeis.org/A169375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24 * S_{n-1} + p(n), where S_k = sum_{i=0}^k a(i), p(0) =
 *            1, p(n) = 2 for 1 ≤ n ≤ 30, p(31) = 1, p(n) = 0 for n ≥ 32
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t a_val = (seq_t)1;   /* a(0) */
    seq_t sum   = a_val;      /* S_0 = a(0) */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t p;
        if (i <= 30)
            p = (seq_t)2;
        else if (i == 31)
            p = (seq_t)1;
        else
            p = (seq_t)0;

        a_val = (seq_t)((seq_t)24 * sum + p);
        sum += a_val;
    }

    return a_val;
}
