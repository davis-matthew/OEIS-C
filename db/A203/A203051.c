/*
 * A203051  Number of nX2 0..5 arrays with every nonzero element less than or
 *          equal to some horizontal or vertical neighbor.
 * https://oeis.org/A203051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*
 * Formula:
 * a(1)=6, a(2)=286, a(3)=7820, a(4)=177290, a(5)=4078368,
 * a(6)=96511488, a(7)=2282090278, a(8)=53729637298,
 * a(9)=1264907516536, a(10)=29797761325536,
 * a(11)=701982503271882.
 * For n>11:
 * a(n) = 21*a(n-1) - 50*a(n-2) + 2261*a(n-3) + 5179*a(n-4)
 *        + 58301*a(n-5) + 113102*a(n-6) + 454821*a(n-7)
 *        + 550157*a(n-8) + 1071256*a(n-9) + 600336*a(n-10)
 *        + 518400*a(n-11)
 */

static const seq_t init_vals[11] = {
    (seq_t)6ULL,
    (seq_t)286ULL,
    (seq_t)7820ULL,
    (seq_t)177290ULL,
    (seq_t)4078368ULL,
    (seq_t)96511488ULL,
    (seq_t)2282090278ULL,
    (seq_t)53729637298ULL,
    (seq_t)1264907516536ULL,
    (seq_t)29797761325536ULL,
    (seq_t)701982503271882ULL
};

static seq_t *memo = NULL;    /* memoization buffer, 1‑based indices */
static idx_t memo_last = 0;   /* largest n already computed (0 means none) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* offset starts at 1; value for 0 unused */

    /* Allocate / resize buffer if necessary */
    if (memo == NULL || memo_last < n) {
        size_t needed = (size_t)n + 1;           /* include unused index 0 */
        seq_t *tmp = (seq_t *)realloc(memo, needed * sizeof(seq_t));
        if (!tmp)                                 /* allocation failure */
            return (seq_t)0;
        memo = tmp;

        /* Initialise the known first 11 values on first use or when extending */
        for (idx_t i = memo_last + 1; i <= 11 && i <= n; ++i)
            memo[i] = init_vals[i - 1];
    }

    /* Compute any missing values up to n */
    for (idx_t i = memo_last + 1; i <= n; ++i) {
        if (i <= 11) {
            /* already initialized */
        } else {
            memo[i] =
                (seq_t)((seq_t)21    * memo[i - 1]) -
                (seq_t)((seq_t)50    * memo[i - 2]) +
                (seq_t)((seq_t)2261  * memo[i - 3]) +
                (seq_t)((seq_t)5179  * memo[i - 4]) +
                (seq_t)((seq_t)58301 * memo[i - 5]) +
                (seq_t)((seq_t)113102 * memo[i - 6]) +
                (seq_t)((seq_t)454821 * memo[i - 7]) +
                (seq_t)((seq_t)550157 * memo[i - 8]) +
                (seq_t)((seq_t)1071256 * memo[i - 9]) +
                (seq_t)((seq_t)600336 * memo[i - 10]) +
                (seq_t)((seq_t)518400 * memo[i - 11]);
        }
    }

    memo_last = n;
    return memo[n];
}
