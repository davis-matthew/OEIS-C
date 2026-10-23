/*
 * A125350  Number of base 13 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; for n>=1, a(n) is the trace of M^n, where M is the
 *            13×13 adjacency matrix M[i][j] = 1 if |i-j| <= 4, otherwise 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* adjacency matrix – computed once */
    static int init = 0;
    static seq_t M_adj[13][13];

    if (!init)
    {
        init = 1;
        for (idx_t i = 0; i < 13; ++i)
        {
            for (idx_t j = 0; j < 13; ++j)
            {
                seq_t cond = (i > j ? (i - j) : (j - i)) <= 4 ? (seq_t)1 : (seq_t)0;
                M_adj[i][j] = cond;
            }
        }
    }

    if (n == 0) return (seq_t)1;

    /* Mpow holds M^(step) */
    seq_t Mpow[13][13];
    /* initialize to identity (n=0) */
    for (idx_t i = 0; i < 13; ++i)
    {
        for (idx_t j = 0; j < 13; ++j)
            Mpow[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
    }

    /* iterate up to n, multiplying by M_adj each time */
    for (idx_t step = 1; step <= n; ++step)
    {
        seq_t tmp[13][13];
        for (idx_t i = 0; i < 13; ++i)
        {
            for (idx_t j = 0; j < 13; ++j)
            {
                seq_t sum = (seq_t)0;
                for (idx_t k = 0; k < 13; ++k)
                {
                    if (M_adj[k][j])
                        sum += Mpow[i][k];
                }
                tmp[i][j] = sum;
            }
        }
        /* copy back to Mpow */
        for (idx_t i = 0; i < 13; ++i)
            for (idx_t j = 0; j < 13; ++j)
                Mpow[i][j] = tmp[i][j];
    }

    /* trace of Mpow is the answer for n>=1 */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < 13; ++i)
        result += Mpow[i][i];

    return result;
}
