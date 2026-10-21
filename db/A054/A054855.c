/*
 * A054855  Number of ways to tile a 5 X n area with 1 X 1 and 2 X 2 tiles.
 * https://oeis.org/A054855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) - 2*a(n-3) - 3*a(n-4); initial
 *            values: a(0)=1, a(1)=1, a(2)=8, a(3)=21
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation buffer --------------------------------------------------- */
    static seq_t *dp = NULL;          /* array of computed values    */
    static idx_t dp_len   = 0;        /* allocated size (in elements)*/
    static idx_t max_computed = 0;    /* highest computed index so far */

    /* ensure capacity for indices 0 ... n (and at least 4 for base cases) */
    size_t new_len = (size_t)(n + 1);
    if (new_len < 4) new_len = 4;                 /* at least 4 elements */
    if (new_len > dp_len) {
        /* realloc must be cast in C++ to avoid implicit conversion */
        seq_t *tmp = (seq_t *)realloc(dp, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                /* allocation failed */
        dp = tmp;
        dp_len = new_len;
    }

    /* initialise the first four terms on the first call (or if array grew) */
    if (max_computed == 0) {
        dp[0] = (seq_t)1;   /* a(0) */
        dp[1] = (seq_t)1;   /* a(1) */
        if (dp_len > 2) dp[2] = (seq_t)8;    /* a(2) */
        if (dp_len > 3) dp[3] = (seq_t)21;   /* a(3) */
        max_computed = (dp_len > 4) ? 3 : (idx_t)(dp_len - 1);
    }

    /* already computed? ---------------------------------------------------- */
    if (n <= max_computed)
        return dp[n];

    /* compute missing terms up to n using the recurrence ------------------- */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* use temporary variables to avoid multiple evaluation of dp[i-]   */
        seq_t term1 = 2 * dp[i - 1] + 7 * dp[i - 2];
        seq_t term2 = 2 * dp[i - 3] + 3 * dp[i - 4];
        dp[i] = term1 - term2;               /* guaranteed non‑negative   */
    }

    max_computed = n;
    return dp[n];
}
