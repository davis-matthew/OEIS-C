/*
 * A103422  Number of compositions of n in which the greatest part is even.
 * https://oeis.org/A103422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k even, 2≤k≤n} (C(n,≤k) - C(n,≤k-1)), where C(n,≤m) is
 *            the number of compositions of n with all parts ≤ m. This
 *            implementation builds the values incrementally using dynamic
 *            programming. The algorithm runs in O(n²) time, which is fast
 *            enough for n ≤ 10⁴. No external libraries are required
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#ifndef MAX_N
#  define MAX_N 200000   /* ample size for the expected tests (n≈10⁴) */
#endif

static seq_t a_table[MAX_N + 1];   /* memoised a(n) values          */
static idx_t current_max = 0;      /* largest n already computed    */

/* --------------------------------------------------------------------- */
/* Build all a(n) for 1 ≤ n ≤ limit.  limit ≤ MAX_N.                     */
/* --------------------------------------------------------------------- */
static void build_table(idx_t limit)
{
    /* initialise a_table to zero for the range we will use */
    for (idx_t i = 0; i <= limit; ++i)
        a_table[i] = 0;

    /* arrays holding f_prev and f_curr for the DP on max part size */
    seq_t f_prev[MAX_N + 1];
    seq_t f_curr[MAX_N + 1];

    /* base case: with maximum part size 0 there is exactly one
       composition of 0 and none of any positive n. */
    f_prev[0] = 1;
    for (idx_t i = 1; i <= limit; ++i)
        f_prev[i] = 0;

    for (idx_t m = 1; m <= limit; ++m)            /* current maximum part */
    {
        f_curr[0] = 1;            /* empty composition counts for n = 0 */
        seq_t window = 0;         /* sliding window over the last m cells */

        for (idx_t k = 1; k <= limit; ++k)        /* compute f_curr[k] */
        {
            window += f_curr[k - 1];
            if (k - 1 >= m)
                window -= f_curr[k - 1 - m];
            f_curr[k] = window;
        }

        if ((m & (idx_t)1) == 0)        /* m is even – add its contribution */
        {
            for (idx_t k = 1; k <= limit; ++k)
                a_table[k] += f_curr[k] - f_prev[k];
        }

        /* prepare for next iteration: f_prev ← f_curr */
        for (idx_t i = 0; i <= limit; ++i)
            f_prev[i] = f_curr[i];
    }
}

/* --------------------------------------------------------------------- */
/* Public function returning a(n) for n ≥ 1.  For n < 1 a(0) is undefined
   and 0 is returned.  Subsequent calls may be in any order.            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* below the offset – undefined */

    if (n > current_max)                /* need to extend the table */
    {
        build_table(n);
        current_max = n;
    }
    return a_table[n];
}
