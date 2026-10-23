/*
 * A134136  a(n) = 2*a(n-2) + 4*a(n-3), with initial terms 0, 1, 1.
 * https://oeis.org/A134136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2*a(n-2) + 4*a(n-3), a(0)=0, a(1)=1, a(2)=1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static size_t memo_len = 0;
static size_t memo_last = 0;  /* last index that has been computed */

/* Ensure memo array can hold indices up to need-1 */
static void ensure_memo(size_t need)
{
    if (memo_len < need) {
        size_t new_len = memo_len ? memo_len * 2 : 64;
        if (new_len < need) new_len = need;
        void *ptr = realloc(memo, new_len * sizeof(seq_t));
        memo = (seq_t *)ptr;
        memo_len = new_len;
    }
}

/* The main function required by the problem statement */
seq_t a(idx_t n)
{
    /* handle the trivial base cases directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;

    size_t idx = (size_t)n;          /* convert to a C index type */
    if (memo == NULL) {
        /* first time: allocate enough space for indices up to n */
        size_t init_len = idx + 1;
        if (init_len < 3) init_len = 3;
        memo = (seq_t *)malloc(init_len * sizeof(seq_t));
        memo_len = init_len;
        /* initialize the base values */
        memo[0] = (seq_t)0;
        memo[1] = (seq_t)1;
        memo[2] = (seq_t)1;
        memo_last = 2;
    } else {
        /* make sure the array is large enough */
        ensure_memo(idx + 1);
    }

    /* If we have already computed up to idx, just return */
    if (idx <= memo_last)
        return memo[idx];

    /* Compute missing values up to idx */
    for (size_t i = memo_last + 1; i <= idx; ++i) {
        memo[i] = memo[i - 2] * (seq_t)2 + memo[i - 3] * (seq_t)4;
    }
    memo_last = idx;
    return memo[idx];
}
