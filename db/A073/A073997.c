/*
 * A073997  Number of strings of length n over GF(4) with trace 1 and subtrace
 *          0.
 * https://oeis.org/A073997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   the number of length‑n strings over GF(4) with trace 1 * and
 *            subtrace 0 is obtained by dynamic programming on all * (trace,
 *            subtrace) pairs in GF(4). The recurrence for the * bivariate
 *            counts a(n; t, s) is * * a(n; t, s) = Σ_{x∈GF(4)} a(n-1; t-x,
 *            s-(t-x)*x) * * with addition and subtraction in GF...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* multiplication table for GF(4) with elements:
 * 0 → 0 , 1 → 1 , 2 → α , 3 → α+1   (field defined by X²+X+1) */
static const unsigned char mul[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 2, 3},
    {0, 2, 3, 1},
    {0, 3, 1, 2}
};

/* Global state for memoisation (static for any order of calls). */
static seq_t *dp = NULL;          /* flat array: dp[row*16 + (t*4 + s)] */
static size_t dp_capacity = 0;    /* number of rows * 16 that we can access */
static idx_t  dp_len = 0;         /* maximum n for which dp[n] is computed */

/* Return a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */

    /* Resize storage if necessary. */
    if ((size_t)n + 1 > dp_capacity) {
        size_t new_capacity = dp_capacity ? dp_capacity * 2 : 16;
        if (new_capacity < (size_t)n + 1) new_capacity = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_capacity * 16 * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        dp = tmp;
        /* initialise new rows to zero */
        memset(dp + dp_capacity * 16, 0,
               (new_capacity - dp_capacity) * 16 * sizeof(seq_t));
        dp_capacity = new_capacity;
    }

    /* Initialise base case if not yet done. */
    if (dp_len == 0) {
        /* dp[1] : all traces 0..3 with subtrace 0 equal 1, others 0 */
        for (int t = 0; t < 4; ++t)
            for (int s = 0; s < 4; ++s)
                dp[1 * 16 + t * 4 + s] = (seq_t)0;
        for (int t = 0; t < 4; ++t)
            dp[1 * 16 + t * 4 + 0] = (seq_t)1;
        dp_len = 1;
    }

    /* Extend DP table up to requested n. */
    for (idx_t cur = dp_len + 1; cur <= n; ++cur) {
        for (int t = 0; t < 4; ++t) {
            for (int s = 0; s < 4; ++s) {
                seq_t sum = 0;
                for (int x = 0; x < 4; ++x) {
                    int tp = t ^ x;                         /* t - x */
                    int sp = s ^ mul[tp][x];                /* s - tp*x */
                    sum += dp[(cur - 1) * 16 + tp * 4 + sp];
                }
                dp[cur * 16 + t * 4 + s] = sum;
            }
        }
    }
    dp_len = n;  /* we have filled rows up to n */

    /* Return the desired value: trace = 1 (index 1), subtrace = 0 */
    return dp[n * 16 + 1 * 4 + 0];
}
