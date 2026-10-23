/*
 * A108246  Number of labeled 2-regular graphs with no multiple edges, but
 *          loops are allowed (i.e., each vertex is endpoint of two (usual)
 *          edges or one loop).
 * https://oeis.org/A108246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=a(2)=1; * Recurrence (valid for n≥0): * a(n+3) = ((n^2
 *            + 3*n + 2)*a(n) * - (2*n + 4)*a(n+1) * + (2*n + 6)*a(n+2)) / 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;   /* dynamic array of computed terms   */
    static idx_t dp_size   = 0; /* current capacity of dp            */
    static idx_t computed  = 0; /* largest index that has been computed */

    /* ----------------------------------------------------------- */
    /* Grow the array if needed (powers of two for efficiency)    */
    if (dp == NULL) {
        dp_size = 4;                         /* at least to hold indices 0 .. 3   */
        dp      = (seq_t *)malloc(dp_size * sizeof(seq_t));
    }
    while (dp_size <= n) {
        dp_size <<= 1;
        dp        = (seq_t *)realloc(dp, dp_size * sizeof(seq_t));
    }

    /* ----------------------------------------------------------- */
    /* Initialise the first three terms (once)                     */
    if (computed == 0) {                      /* first call or freshly initialised */
        dp[0] = 1;
        dp[1] = 1;
        dp[2] = 1;
        computed = 2;
    }

    /* ----------------------------------------------------------- */
    /* Compute missing terms up to n using the recurrence         */
    for (idx_t k = computed + 1; k <= n; ++k) {
        /* Recurrence uses a(k-3), a(k-2), a(k-1) with index n = k-3 */
        idx_t idx = k - 3;                    /* n in the recurrence */

        seq_t a_n   = dp[idx];
        seq_t a_n1  = dp[idx + 1];
        seq_t a_n2  = dp[idx + 2];

        /* Coefficients (all small integers) */
        seq_t coeff   = (seq_t)idx * (seq_t)idx + (seq_t)3 * (seq_t)idx + 2;
        seq_t term2   = (seq_t)2 * (seq_t)idx + 4;
        seq_t term3   = (seq_t)2 * (seq_t)idx + 6;

        /* Compute numerator = coeff*a_n - term2*a_n1 + term3*a_n2   */
        seq_t num = coeff * a_n;          /* first term, safe intermediate  */
        num += term3 * a_n2;              /* add the positive term          */
        num -= term2 * a_n1;              /* subtract the middle term       */

        dp[k] = num >> 1;                 /* divide by 2 (integer division) */
    }

    computed = n;
    return dp[n];
}
