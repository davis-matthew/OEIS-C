/*
 * A180875  Sum_{j>=1} j^n*2^j/binomial(2*j,j) = r_n*Pi/2 + s_n with integer
 *          r_n and s_n; sequence gives s_n.
 * https://oeis.org/A180875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n ≥ 1 a(n) = a(n‑1) + 1 + Σ_{m=1..n} binomial(n,m)
 *            * a(n‑m)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *memo = NULL;      /* memoised values        */
static size_t memo_cap = 0;     /* allocated size (bytes) */
static idx_t  memo_len = 0;     /* number of stored values */

/* Ensure that ``memo`` can address indices from 0 … n. */
static void
ensure_capacity(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (memo == NULL) {
        memo_cap = needed > 16 ? needed : 16;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        memo_len = 0;
    } else if (needed > memo_cap) {
        size_t new_cap = memo_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        memo = tmp;
        memo_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* return cached value if available */
    if (n < memo_len) return memo[n];

    /* grow storage if necessary */
    ensure_capacity(n);

    /* compute all missing values up to n */
    for (idx_t i = memo_len; i <= n; ++i) {
        if (i == 0) {                 /* a(0) = 1 */
            memo[0] = (seq_t)1;
            continue;
        }

        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;      /* binomial(i,0) */

        for (idx_t m = 1; m <= i; ++m) {
            /* binom(i,m) = binom(i,m‑1) * (i‑m+1) / m   */
            binom = binom * (seq_t)(i - m + 1) / (seq_t)m;
            sum   = sum + binom * memo[i - m];
        }

        memo[i] = memo[i - 1] + (seq_t)1 + sum;
    }

    memo_len = n + 1;
    return memo[n];
}
