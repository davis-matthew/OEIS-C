/*
 * A072329  a(n+1) = 3*a(n-2) + 2*a(n-1), a(n)=x^n+y^n+z^n.
 * https://oeis.org/A072329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-3) + 2*a(n-2) for n ≥ 3, with a(0) = 3, a(1) = 0,
 *            a(2) = 4
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;      /* cache of computed terms   */
static idx_t  memo_len = 0;     /* number of terms stored   */
static idx_t  memo_cap = 0;     /* allocated capacity       */

/* ensure memo has room for indices 0 … n inclusive */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= memo_cap)
        return;
    idx_t new_cap = (memo_cap == 0) ? 8 : memo_cap * 2;
    while (new_cap <= n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(memo, (size_t)new_cap * sizeof(seq_t));
    memo = tmp;
    memo_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < memo_len)
        return memo[n];

    ensure_capacity(n);

    /* initialise the first three terms if not yet done */
    if (memo_len == 0) { memo[0] = 3; memo_len = 1; }
    if (memo_len == 1) { memo[1] = 0; memo_len = 2; }
    if (memo_len == 2) { memo[2] = 4; memo_len = 3; }

    /* compute missing terms in ascending order */
    for (idx_t i = memo_len; i <= n; ++i) {
        memo[i] = 3 * memo[i - 3] + 2 * memo[i - 2];
    }
    memo_len = n + 1;
    return memo[n];
}
