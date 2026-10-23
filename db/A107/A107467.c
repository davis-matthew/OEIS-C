/*
 * A107467  a(n)= 3*a(n-1) +a(n-2) -3*a(n-3) +a(n-4)+ 2*a(n-5) -a(n-6)
 *          -a(n-7).
 * https://oeis.org/A107467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 3*a(n-3) + a(n-4) + 2*a(n-5) - a(n-6)
 *            - a(n-7) Initial values: a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=3,
 *            a(5)=5, a(6)=8, a(7)=23
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 24, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc, abort */

static const seq_t init_values[8] = {
    (seq_t)0, (seq_t)1, (seq_t)1, (seq_t)2,
    (seq_t)3, (seq_t)5, (seq_t)8, (seq_t)23
};

static seq_t *dp = NULL;          /* memoisation array */
static idx_t  dp_size = 0;        /* allocated size of dp */
static idx_t  last_computed = 8;  /* first index that is NOT yet computed */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Return directly if n falls within the pre‑defined initial block */
    if (n < 8)
        return init_values[n];

    /* Allocate or grow the memoisation array if needed.  The cast
       ensures the code compiles both as C and as C++. */
    if (!dp) {
        idx_t needed = n + 1;
        if (needed < 8) needed = 8;
        dp = (seq_t *)malloc((size_t)needed * sizeof(seq_t));
        if (!dp) abort();
        for (idx_t i = 0; i < 8; ++i)
            dp[i] = init_values[i];
        dp_size = needed;
        last_computed = 8;
    } else if (dp_size <= n) {
        idx_t new_size = (dp_size > 0) ? dp_size * 2 : 8;
        while (new_size <= n) new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(dp, (size_t)new_size * sizeof(seq_t));
        if (!tmp) abort();
        dp = tmp;
        dp_size = new_size;
    }

    /* Compute any missing terms up to n.  The recurrence uses the
       previous seven values, so we can evaluate iteratively. */
    for (idx_t i = last_computed; i <= n; ++i) {
        dp[i] =
            3 * dp[i-1]     + dp[i-2]
          - 3 * dp[i-3]     + dp[i-4]
          + 2 * dp[i-5]     - dp[i-6] - dp[i-7];
    }
    last_computed = n + 1;
    return dp[n];
}
