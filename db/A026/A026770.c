/*
 * A026770  a(n) = T(2n,n), T given by A026769.
 * https://oeis.org/A026770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>0
   Let f[i][j] be the number of lattice paths from (0,0) to (i,j)
   with steps (1,0),(0,1) and (1,1) only when the starting point is
   below the diagonal y=x.  Then
      f[0][*] = f[*][0] = 1,
      f[i][j] = f[i-1][j] + f[i][j-1] + (j<i ? f[i-1][j-1] : 0),
   and the desired term is a(n) = f[n][n].  */
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
static seq_t *dp = nullptr;
static size_t dp_cap = 0;
#else
static seq_t *dp = NULL;
static size_t dp_cap = 0;
#endif

/* Computes a(n) = T(2n,n) for n ≥ 0 */
seq_t a(idx_t n)
{
    /* base case */
    if (n == 0) return (seq_t)1;

    /* ensure storage for 0…n */
    size_t need = (size_t)n + 1;
    if (dp_cap < need) {
#ifdef __cplusplus
        seq_t *newdp = new seq_t[need];
        delete[] dp;          /* safe even if dp is nullptr */
#else
        seq_t *newdp = (seq_t *)realloc(dp, need * sizeof(seq_t));
        if (!newdp) {
            /* allocation failed – return 0 as an error indicator */
            return (seq_t)0;
        }
#endif
        dp = newdp;
        dp_cap = need;
    }

    /* initialise row 0: f[0][j] = 1 for all j */
    for (idx_t j = 0; j <= n; ++j)
        dp[j] = (seq_t)1;

    /* dynamic programming rows 1 … n, in‑place */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t prev_diag = dp[0];   /* f[i‑1][0] */
        dp[0] = (seq_t)1;          /* f[i][0] = 1 */

        for (idx_t j = 1; j <= n; ++j) {
            seq_t temp = dp[j];           /* f[i‑1][j]   */
            seq_t val  = temp + dp[j-1];   /* +f[i][j‑1] */
            if (j < i) val += prev_diag;   /* +f[i‑1][j‑1] if below diagonal */
            prev_diag = temp;              /* next diagonal value */
            dp[j] = val;
        }
    }

    /* after last row dp[n] holds f[n][n] */
    return dp[n];
}
