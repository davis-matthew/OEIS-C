/*
 * A011529  Stirling numbers of first kind S1(19,n).
 * https://oeis.org/A011529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   Stirling numbers of the first kind for n = 19. A011529 gives the
 *            coefficients of (x)(x+1)…(x+18), i.e. a(k) = s(19,k), 1 ≤ k ≤
 *            19, with a(k)=0 for k>19
 * Range:     i8 none, i16 none, i32 none, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static table for s(19,k) computed via recurrence s(n,k)=s(n-1,k-1)-(n-1)*s(n-1,k) */
    static bool init = false;
    static seq_t st[20][20];   /* 0‑based indices; only st[19][1..19] needed */

    if (!init)
    {
        st[0][0] = 1;
        for (idx_t i = 1; i <= 19; ++i)
        {
            for (idx_t k = 1; k <= i; ++k)
            {
                seq_t term1 = st[i-1][k-1];
                seq_t term2 = (seq_t)(i-1) * st[i-1][k];
                st[i][k] = term1 - term2;
            }
        }
        init = true;
    }

    if (n == 0 || n > 19) return 0;
    return st[19][(idx_t)n];
}
