/*
 * A181233  Number of nX(n+3) binary matrices with all rows unique and all
 *          columns unique.
 * https://oeis.org/A181233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{s=1..n} (-1)^{(n+3)+1+s} * stirling1(n,s) *
 *            falling_factorial(2^s, n+3)
 *            (falling_factorial(x,k)=x*(x-1)*...*(x-k+1))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* Count of Stirling numbers of the first kind (unsigned) :
   c(i,j) = c(i-1,j-1) + (i-1) * c(i-1,j)   for i>0, j>0   with c(0,0)=1. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* sequence defined for n≥1 */

    /* m = n+3, the number of columns */
    const idx_t m = n + 3;

    /* Allocate two rows for the dynamic programming of Stirling numbers */
    seq_t *prev = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* initialize Stirling row for i=0 */
    for (idx_t i = 0; i <= n; ++i) prev[i] = 0;
    prev[0] = 1;

    /* compute the Stirling numbers for i = 1..n  (only the final row is needed) */
    for (idx_t i = 1; i <= n; ++i) {
        curr[0] = 0;
        for (idx_t j = 1; j <= i; ++j) {
            curr[j] = prev[j - 1] + (seq_t)(i - 1) * prev[j];
        }
        /* zero trailing entries (they are not accessed later) */
        for (idx_t j = i + 1; j <= n; ++j) curr[j] = 0;
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* sum over s from s_min to n, where s_min is the smallest s with 2^s ≥ m */
    idx_t s_min = 0;
    for (idx_t s = 0; ((seq_t)1 << s) < (seq_t)m; ++s) s_min = s + 1;

    seq_t pos = 0;          /* accumulate terms with positive sign */
    seq_t neg = 0;          /* accumulate terms with negative sign */

    for (idx_t s = s_min; s <= n; ++s) {
        /* 2^s as a big integer */
        seq_t two_pow_s = big_pow((seq_t)2, s);

        /* falling factorial (2^s)_m = (2^s)(2^s-1)...(2^s-m+1) */
        seq_t prod = (seq_t)1;
        for (idx_t i = 0; i < m; ++i) {
            prod *= (two_pow_s - (seq_t)i);
        }

        /* term = c(n,s) * prod */
        seq_t term = prev[s] * prod;

        /* sign: (-1)^{(m+1)+s} */
        if (((m + 1 + s) & 1) != 0) {
            neg += term;
        } else {
            pos += term;
        }
    }

    free(prev);
    free(curr);

    /* the final count is pos - neg (pos ≥ neg for the combinatorial sequence) */
    return pos - neg;
}
