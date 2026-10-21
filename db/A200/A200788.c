/*
 * A200788  Number of 0..n arrays x(0..5) of 6 elements without any two
 *          consecutive increases.
 * https://oeis.org/A200788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) * +
 *            21*a(n-5) - 7*a(n-6) + a(n-7) for n > 7 * Initial values:
 *            a(1)=64, a(2)=622, a(3)=3136, a(4)=11100, * a(5)=31395,
 *            a(6)=75992, a(7)=164004
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 44, u64 n <= 138, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[7] = {
    (seq_t)64,
    (seq_t)622,
    (seq_t)3136,
    (seq_t)11100,
    (seq_t)31395,
    (seq_t)75992,
    (seq_t)164004
};

/* cached sequence values, zero‑based (memo[0] = a(1)) */
static seq_t *memo   = NULL;
static idx_t memo_cap = 0;  /* capacity of memo array */
static idx_t memo_len = 0;  /* number of values that have been computed */

/* Ensure memo can hold at least 'need' elements (index 0..need-1). */
static void ensure_capacity(idx_t need)
{
    if (need <= memo_cap) return;
    /* grow to exactly 'need' elements – small growth cost is fine */
    seq_t *ptr = NULL;
    if (memo == NULL)
        ptr = (seq_t *)malloc((size_t)need * sizeof(seq_t));
    else
        ptr = (seq_t *)realloc(memo, (size_t)need * sizeof(seq_t));
    if (!ptr) abort();          /* out of memory */
    memo = ptr;
    memo_cap = need;
}

/* a(n) – use memoised computation.  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1, undefined for 0 */
    if (n <= (idx_t)7) return init_vals[n-1];

    /* extend cache up to n */
    if (memo_len < n) {
        ensure_capacity(n);
        for (idx_t i = memo_len; i < n; ++i) {
            if (i < (idx_t)7) {
                memo[i] = init_vals[i];
            } else {
                /* recurrence using previous seven values */
                seq_t val = 7 * memo[i-1];
                val -= 21 * memo[i-2];
                val += 35 * memo[i-3];
                val -= 35 * memo[i-4];
                val += 21 * memo[i-5];
                val -= 7 * memo[i-6];
                val += memo[i-7];
                memo[i] = val;
            }
        }
        memo_len = n;
    }
    return memo[n-1];
}
