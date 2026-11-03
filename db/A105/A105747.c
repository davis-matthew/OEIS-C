/*
 * A105747  Number of ways to use the elements of {1,..,k}, 0<=k<=2n, once
 *          each to form a collection of n (possibly empty) lists, each of
 *          length at most 2.
 * https://oeis.org/A105747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = (4*n+11)*a(n+2) – (4*n+9)*a(n+1) – a(n). Initial
 *            values: a(0)=1, a(1)=4, a(2)=23
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 346 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Ensure an array of seq_t is large enough to hold indexes [0..min_needed] */
static void ensure_seq_array(seq_t **arr, size_t *cap, size_t min_needed)
{
    if (*cap <= min_needed) {
        size_t newcap = (*cap) ? (*cap) * 2 : 16;
        if (newcap <= min_needed) newcap = min_needed + 1;
        *arr = mem_resize(seq_t, *arr, newcap);
        for (size_t i = *cap; i < newcap; ++i) (*arr)[i] = 0;
        *cap = newcap;
    }
}

static seq_t *dp = NULL;     /* memoization buffer */
static size_t dp_cap = 0;    /* allocated capacity */
static idx_t  dp_max = 0;    /* highest index already computed */

/* initialise the first three terms once */
static void init(void) {
    if (!dp) {
        /* guarantee space for indices 0,1,2 */
        ensure_seq_array(&dp, &dp_cap, 3);
        dp[0] = (seq_t)1;
        dp[1] = (seq_t)4;
        dp[2] = (seq_t)23;
        dp_max = 2;
    }
}

/* Return term a(n) for all n ≥ 0 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)23;

    if (!dp) init();                  /* first use of the table */

    if (n > dp_max) {                 /* extend table if necessary */
        ensure_seq_array(&dp, &dp_cap, (size_t)(n + 1));
        for (idx_t i = dp_max + 1; i <= n; ++i) {
            /* i = n in the recurrence, m = i-3 */
            seq_t coeff1 = (seq_t)(4 * (i - 3) + 11);   /* (4m+11) */
            seq_t coeff2 = (seq_t)(4 * (i - 3) + 9);    /* (4m+9) */
            dp[i] = coeff1 * dp[i - 1] - coeff2 * dp[i - 2] - dp[i - 3];
        }
        dp_max = n;
    }
    return dp[n];
}
