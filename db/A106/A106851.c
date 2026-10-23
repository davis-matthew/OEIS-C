/*
 * A106851  Expansion of (-3*x^3 - 7*x^2 + 2*x)/((1-4*x-x^2)*(1-4*x+x^2)).
 * https://oeis.org/A106851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 16*a(n-2) + a(n-4), a(0)=0, a(1)=2, a(2)=9,
 *            a(3)=37
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static storage for memoisation */
static seq_t *dp = NULL;          /* computed terms */
static idx_t latestIndex = 0;     /* index of highest computed term */
static size_t capacity = 0;       /* current allocation size */
static bool initialized = false;  /* whether dp has been initialised */

/* Ensure we have computed a[0..n] */
static void ensure_up_to(idx_t n)
{
    /* Initialise the table on first use, guaranteeing space for the 4 base cases */
    if (!initialized) {
        capacity = (size_t)(n + 1 > 4 ? n + 1 : 4);          /* at least 4 entries */
        dp = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!dp) abort();                                   /* out of memory */
        dp[0] = 0;
        dp[1] = 2;
        dp[2] = 9;
        dp[3] = 37;
        latestIndex = 3;
        initialized = true;
    }

    /* Grow the allocation if necessary */
    if ((size_t)(n + 1) > capacity) {
        capacity = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(dp, capacity * sizeof(seq_t));
        if (!tmp) abort();
        dp = tmp;
    }

    /* Compute new terms only if needed */
    for (idx_t i = latestIndex + 1; i <= n; ++i) {
        seq_t term1 = 8 * dp[i - 1];    /* 8*a(i-1)  -- safe for required range */
        seq_t term2 = 16 * dp[i - 2];   /* 16*a(i-2) */
        seq_t val   = term1 - term2;    /* 8*a(i-1) - 16*a(i-2) */
        val += dp[i - 4];               /* + a(i-4) */
        dp[i] = val;
    }

    if (n > latestIndex) {
        latestIndex = n;
    }
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return dp[n];
}
