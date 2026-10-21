/*
 * A212687  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          2|w-x|<n+|y-z|.
 * https://oeis.org/A212687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A212687 */
/* a(n) = number of (w,x,y,z) with 1≤w,x,y,z≤n and 2*|w-x| < n + |y-z| */

#include <stdint.h>

/* seq_t and idx_t are provided by the build environment */
seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t nn     = (seq_t)n;
    seq_t nn2    = nn * nn;           /* n^2 */
    int64_t bound = (int64_t)n - 1;   /* n-1 in signed form */

    for (idx_t d = 0; d < n; ++d)
    {
        seq_t pairs = (d == 0) ? nn : (2 * (nn - (seq_t)d));

        int64_t t = 2 * (int64_t)d - (int64_t)n;  /* t = 2*d - n */

        seq_t count_yz;
        if (t < 0)
            count_yz = nn2;                       /* all n^2 pairs (y,z) satisfy */
        else if (t >= bound)
            count_yz = 0;                         /* no pair satisfies */
        else
        {
            seq_t L = (seq_t)(t + 1);             /* L = t+1, 1 ≤ L ≤ n-1 */
            seq_t m = nn - L;                    /* m = n - L, 1 ≤ m ≤ n-1 */
            count_yz = m * (m + 1);              /* (n-L)*(n-L+1) */
        }

        total += pairs * count_yz;
    }

    return total;
}
