/*
 * A172345  Triangle t(n,k) read by rows: fibonomial ratios c(n)/(c(k)*c(n-k))
 *          where c are partial products of a generalized Fibonacci sequence
 *          with multiplier m=7.
 * https://oeis.org/A172345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = t(r,k) where r(r+1)/2 <= n < (r+1)(r+2)/2, k = n -
 *            r(r+1)/2; t(r,k) = c(r)/(c(k)*c(r-k)) with c(i) =
 *            product_{j=1..i} F(j), F(0)=0, F(1)=1, F(n)=7*F(n-1)+F(n-2)
 *            (generalized Fibonacci with m=7)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 30, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= n)
        ++r;
    --r;                                          /* r now correct */
    idx_t offset = (r * (r + 1)) / 2;              /* index where row r starts */
    idx_t k = n - offset;                          /* position in that row */

    /* ---------- precompute generalized Fibonacci numbers F[0..r] ---------- */
    seq_t *F = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!F)
        return 0;                                 /* allocation failed */
    F[0] = 0;
    if (r >= 1)
        F[1] = 1;
    for (idx_t i = 2; i <= r; ++i)
        F[i] = F[i - 1] * 7 + F[i - 2];

    /* ---------- build rows up to row r, keeping only the previous row --------- */
    seq_t *prev = NULL;
    size_t prev_len = 0;
    for (idx_t cur = 0; cur <= r; ++cur)
    {
        size_t cur_len = cur + 1;
        seq_t *curr = (seq_t *)malloc(cur_len * sizeof(seq_t));
        if (!curr) { free(prev); free(F); return 0; }
        curr[0] = 1;                               /* first element of every row = 1 */
        for (idx_t kk = 1; kk <= cur; ++kk)
        {
            seq_t num = prev[kk - 1] * F[cur];     /* prev[k-1] * F[cur]   */
            seq_t den = F[kk];                     /*   F[k]               */
            curr[kk] = num / den;                  /* exact integer division */
        }
        free(prev);
        prev = curr;
        prev_len = cur_len;
    }

    seq_t result = prev[k];
    free(prev);
    free(F);
    return result;
}
