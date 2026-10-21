/*
 * A210005  Number of (n+1) X 2 0..2 arrays containing all values 0..2 with
 *          every 2 X 2 subblock having two or three distinct values, and new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A210005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 16*a(n-2) - 34*a(n-3) - 12*a(n-4), with
 *            initial terms a(1)=6, a(2)=88, a(3)=893, a(4)=8217. The function
 *            uses memoisation so that any index n ≥ 1 can be queried in any
 *            order
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset; not used */

    static seq_t *dp = NULL;              /* memoisation array */
    static size_t sz = 0;                 /* current allocation size */

    /* initialise the base of the recurrence (indices 1..4) */
    if (dp == NULL) {
        sz = 5; /* indices 0..4 */
        dp = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!dp) return (seq_t)0;          /* allocation failed */
        dp[0] = (seq_t)0;
        dp[1] = (seq_t)6;
        dp[2] = (seq_t)88;
        dp[3] = (seq_t)893;
        dp[4] = (seq_t)8217;
    }

    /* if we need more terms than we have, grow the array and fill them */
    if ((size_t)n >= sz) {
        size_t new_sz = (size_t)n + 1;     /* allocate up to index n */
        seq_t *tmp = (seq_t *)realloc(dp, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        dp = tmp;
        /* compute new entries from the first missing index (usually 5) */
        for (size_t i = sz; i < new_sz; ++i) {
            seq_t t = ((seq_t)11) * dp[i-1]     /* 11*a(i-1) */
                    - ((seq_t)16) * dp[i-2]     /*   16*a(i-2) */
                    - ((seq_t)34) * dp[i-3]     /*   34*a(i-3) */
                    - ((seq_t)12) * dp[i-4];    /*   12*a(i-4) */
            dp[i] = t;
        }
        sz = new_sz;
    }

    return dp[(size_t)n];
}
