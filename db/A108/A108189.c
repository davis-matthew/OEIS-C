/*
 * A108189  a(n) = (n-1)*(a(n-2)+a(n-3)).
 * https://oeis.org/A108189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*(a(n-2)+a(n-3)) Recurrence with initial values:
 *            a(1)=0, a(2)=1, a(3)=1 offset: 1
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *dp = NULL;          /* memoization array (1‑based) */
static size_t dp_cap = 0;         /* capacity of dp */
static idx_t dp_upto = 0;         /* largest n computed so far (<= dp_cap-1) */

static void ensure_capacity(idx_t n)
{
    if (dp_cap >= (size_t)(n + 1))
        return;                   /* already enough */

    size_t new_cap = dp_cap ? dp_cap * 2 : 4;
    while (new_cap <= (size_t)n)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);            /* realloc failed – terminate */
    dp = tmp;
    dp_cap = new_cap;
}

static void init_sequence(void)
{
    /* Ensure base values for n = 1,2,3 */
    if (dp_upto >= 3) return;
    ensure_capacity(3);
    dp[1] = (seq_t)0;
    dp[2] = (seq_t)1;
    dp[3] = (seq_t)1;
    dp_upto = 3;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* behaviour below offset is irrelevant */
    init_sequence();

    if ((size_t)n > dp_cap - 1)                     /* need more space */
        ensure_capacity(n);

    /* compute until we reach n */
    for (idx_t i = dp_upto + 1; i <= n; ++i) {
        seq_t sum = dp[i - 2] + dp[i - 3];
        dp[i] = (seq_t)(i - 1) * sum;
    }
    dp_upto = n;

    return dp[n];
}
