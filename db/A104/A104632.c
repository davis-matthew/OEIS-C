/*
 * A104632  1/n times A104631(n), the coefficient of x^(2n+1) in the expansion
 *          of (1+x+x^2+x^3+x^4)^n.
 * https://oeis.org/A104632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n-1)*(19*n^2-19*n+2)*a(n-1) +
 *            5*(n-2)*(2*n^2-3*n-1)*a(n-2) - 25*n*(n-2)*(n-3)*a(n-3)) /
 *            (2*n*(2*n+1)*(n-1)) (cubic recurrence for n ≥ 4, a(1)=1, a(2)=2,
 *            a(3)=6)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑based memoisation array: dp[1..max] holds a(1),a(2),… */
    static seq_t *dp = NULL;
    static idx_t max_computed = 0;
    static idx_t allocated = 0;

    if (n == 0) return 0; /* defined only for n ≥ 1 */

    /* initial allocation */
    if (dp == NULL) {
        allocated = 4;                    /* indices 0..3 used, 0 ignored */
        dp = (seq_t *)malloc(allocated * sizeof(seq_t));
        if (dp == NULL) return 0;         /* allocation failure */
        dp[1] = (seq_t)1;
        dp[2] = (seq_t)2;
        dp[3] = (seq_t)6;
        max_computed = 3;
    } else if (n >= allocated) {          /* grow array if needed */
        seq_t *tmp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;
        dp = tmp;
        allocated = n + 1;
    }

    /* compute missing terms up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* use 128‑bit intermediate arithmetic to avoid overflow */
        unsigned __int128 num = 0;
        unsigned __int128 term = 0;
        unsigned __int128 denom = 0;

        /* term1 = (k-1)*(19*k^2-19*k+2)*a(k-1) */
        term = (unsigned __int128)dp[k - 1];
        term *= (unsigned __int128)(k - 1);
        term *= (unsigned __int128)19 * (unsigned __int128)k * (unsigned __int128)k
                - (unsigned __int128)19 * (unsigned __int128)k + (unsigned __int128)2;
        num += term;

        /* term2 = 5*(k-2)*(2*k^2-3*k-1)*a(k-2) */
        term = (unsigned __int128)dp[k - 2];
        term *= (unsigned __int128)(k - 2);
        term *= (unsigned __int128)2 * (unsigned __int128)k * (unsigned __int128)k
                - (unsigned __int128)3 * (unsigned __int128)k - (unsigned __int128)1;
        term *= (unsigned __int128)5;
        num += term;

        /* term3 = 25*k*(k-2)*(k-3)*a(k-3) (subtracted) */
        term = (unsigned __int128)dp[k - 3];
        term *= (unsigned __int128)k;
        term *= (unsigned __int128)(k - 2);
        term *= (unsigned __int128)(k - 3);
        term *= (unsigned __int128)25;
        num -= term;

        /* denom = 2*k*(2*k+1)*(k-1) */
        denom = (unsigned __int128)2 * (unsigned __int128)k
                * ((unsigned __int128)2 * (unsigned __int128)k + (unsigned __int128)1)
                * ((unsigned __int128)k - (unsigned __int128)1);

        dp[k] = (seq_t)(num / denom);       /* division is exact */
    }

    max_computed = n;                       /* update cache */
    return dp[n];
}
