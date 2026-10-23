/*
 * A183314  Number of n X 2 binary arrays with an element zero only if there
 *          are an even number of ones to its left and an even number of ones
 *          above it.
 * https://oeis.org/A183314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 59, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 3*a(n-2) - a(n-3) - a(n-4)  for n>=5
 * with initial values: a(1)=3, a(2)=6, a(3)=13, a(4)=27 */
#include <stdlib.h>

static seq_t *memo = NULL;      /* 1‑based cache, memo[0] unused */
static idx_t memo_size  = 0;    /* allocated size of memo (number of slots) */
static idx_t memo_last  = 0;    /* largest n that has been computed */

/* Ensure that memo[1..n] are computed and memo is large enough. */
static void ensure_idx(idx_t n)
{
    if (n <= memo_last) return;          /* already computed */

    /* Grow the cache if necessary. */
    if (memo_size == 0) memo_size = 16;
    while (memo_size <= n) memo_size <<= 1;

    seq_t *tmp = (seq_t *)realloc(memo, memo_size * sizeof(seq_t));
    memo = tmp;  /* realloc may move the block */

    /* Compute missing terms. */
    for (idx_t i = memo_last + 1; i <= n; ++i) {
        if (i == 1)          memo[i] = (seq_t)3;
        else if (i == 2)     memo[i] = (seq_t)6;
        else if (i == 3)     memo[i] = (seq_t)13;
        else if (i == 4)     memo[i] = (seq_t)27;
        else {
            seq_t a1 = memo[i-1];
            seq_t a2 = memo[i-2];
            seq_t a3 = memo[i-3];
            seq_t a4 = memo[i-4];
            memo[i] = a1 + ((a2 * (seq_t)3)) - a3 - a4;
        }
    }
    memo_last = n;
}

/* Public function as required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 for safety */
    ensure_idx(n);
    return memo[n];
}
