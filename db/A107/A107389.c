/*
 * A107389  Expansion of x*(1-6*x+7*x^2)/((1-x)*(1+x)*(1-5*x+x^2)).
 * https://oeis.org/A107389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=-1, a(3)=2 a(n)=5*a(n-1) - 5*a(n-3) +
 *            a(n-4) for n≥4 (generating function
 *            x*(1-6*x+7*x^2)/((1-x)*(1+x)*(1-5*x+x^2)) )
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 16, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;        /* memoisation buffer */
static idx_t  dp_size = 0;      /* allocated size of dp */
static idx_t  max_computed = 0; /* largest n already computed */

static void init_seq(void)
{
    if (dp == NULL) {
        dp_size = 4;
        dp = (seq_t *)malloc(dp_size * sizeof(seq_t));
        dp[0] = (seq_t)0;
        dp[1] = (seq_t)1;
        dp[2] = (seq_t)-1;
        dp[3] = (seq_t)2;
        max_computed = 3;
    }
}

seq_t a(idx_t n)
{
    init_seq();

    if (n <= max_computed)
        return dp[n];

    /* ensure buffer large enough */
    idx_t required = n + 1;
    if (dp_size < required) {
        idx_t new_size = dp_size ? (dp_size << 1) : 4;
        if (new_size < required)
            new_size = required;
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        dp = tmp;
        dp_size = new_size;
    }

    /* compute missing terms */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        dp[i] = (seq_t)5 * dp[i - 1]
              - (seq_t)5 * dp[i - 3]
              + dp[i - 4];
    }
    max_computed = n;
    return dp[n];
}
