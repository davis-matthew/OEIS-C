/*
 * A170481  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence for OEIS A170481
   a(0) = 1
   For 1 ≤ n < 45 : a(n) = 38 * (a(n-1) + … + a(0)) + 2
   For n ≥ 45   : a(n) = 38 * (a(n-1)+…+a(n-44)) – 741 * a(n-45)
*/

#include <stdlib.h>

static seq_t *memo = NULL;
static size_t memlen = 0;   /* number of values stored so far */
static size_t memcap = 0;   /* current capacity of memo */

/* ensure memo has space for indices up to "needed" (inclusive) */
static void ensure_capacity(size_t needed)
{
    if (needed + 1 <= memcap) return;

    size_t newcap = memcap ? memcap << 1 : 1;
    while (newcap < needed + 1) newcap <<= 1;

    seq_t *tmp = (seq_t *)realloc(memo, newcap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
    memo = tmp;
    memcap = newcap;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_capacity(idx);

    /* compute any missing values in memo */
    for (size_t i = memlen; i <= idx; ++i)
    {
        seq_t sum = 0;

        /* homogeneous part: 38 * sum_{j=1}^{44} a(i-j) */
        for (int j = 1; j <= 44; ++j)
            if (i >= (size_t)j)
                sum += memo[i - j] * 38;

        /* subtract 741 * a(i-45) if applicable */
        if (i >= 45)
            sum -= memo[i - 45] * 741;

        /* add inhomogeneous term for i < 45 */
        if (i < 45)
        {
            if (i == 0)
                sum += 1;
            else
                sum += 2;
        }

        memo[i] = sum;
    }

    memlen = idx + 1;
    return memo[idx];
}
