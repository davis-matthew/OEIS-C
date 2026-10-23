/*
 * A105576  a(n) = 2*a(n-1) - 3*a(n-2) + 2*a(n-3) with a(0) = 3, a(1) = 4,
 *          a(2) = 0.
 * https://oeis.org/A105576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2) + 2*a(n-3) with a(0)=3, a(1)=4,
 *            a(2)=0
 * Range:     i8 n <= 12, i16 n <= 28, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Dynamic storage for previously computed terms */
static seq_t *dp      = NULL;   /* array of computed values  */
static size_t stored  = 0;      /* number of values currently stored (dp[0..stored-1]) */
static size_t capacity = 0;     /* current allocated capacity */

/* Ensure the internal array can hold values up to index n */
static void ensure_capacity(size_t n)
{
    if (n + 1 <= capacity)
        return;
    size_t newcap = capacity < 3 ? 3 : capacity * 2;
    while (n + 1 > newcap)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(dp, newcap * sizeof(seq_t));
    dp = tmp;
    capacity = newcap;
}

/* Initialise the first three terms */
static void init(void)
{
    ensure_capacity(2);
    dp[0] = (seq_t)3;
    dp[1] = (seq_t)4;
    dp[2] = (seq_t)0;
    stored = 3;
}

/* Compute a(n) for any n >= 0, memoising intermediate results */
seq_t a(idx_t n)
{
    /* Lazy initialisation */
    if (!dp)
        init();

    size_t idx = (size_t)n;
    if (idx < stored)
        return dp[idx];

    /* Compute missing terms up to n */
    for (size_t i = stored; i <= idx; ++i) {
        /*  a(i) = 2*a(i-1) - 3*a(i-2) + 2*a(i-3)  */
        seq_t val = (seq_t)2 * dp[i - 1] - (seq_t)3 * dp[i - 2] + (seq_t)2 * dp[i - 3];
        ensure_capacity(i);
        dp[i] = val;
    }
    stored = idx + 1;
    return dp[idx];
}
