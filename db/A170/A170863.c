/*
 * A170863  a(0) = 1, a(1) = 1; a(2^i + j) = 3a(j) + a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1012, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 1;
   a(2^i + j) = 3*a(j) + a(j+1)  for 0 ≤ j < 2^i. */

#include <stdlib.h>   /* realloc, size_t */

static seq_t *dp = NULL;        /* memoisation table */
static size_t dp_cap = 0;       /* allocated size of dp */
static idx_t   dp_max = 0;      /* largest index computed (inclusive) */

/* return largest power of two ≤ x (x ≥ 1) */
static inline idx_t highest_power_of_two(idx_t x)
{
    return 1ULL << (63 - __builtin_clzll(x));
}

seq_t a(idx_t n)
{
    /* Allocate / grow table to contain at least indices 0..n */
    if (dp == NULL) {
        dp_cap = n + 1;
        if (dp_cap < 2) dp_cap = 2;          /* need at least index 1 for the base */
        dp = (seq_t *)realloc(dp, dp_cap * sizeof(seq_t));
        if (dp == NULL) return 0;            /* allocation failed */
        dp_max = 0;
    } else if ((size_t)n >= dp_cap) {
        size_t new_cap = dp_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;
        dp = tmp;
        dp_cap = new_cap;
        /* dp_max stays unchanged */
    }

    /* Ensure the two base values are present.  Overwrite if they were already
       computed – they are always 1. */
    dp[0] = 1;
    if (dp_cap > 1)
        dp[1] = 1;

    /* If the requested value has already been computed, just return it. */
    if (n <= dp_max) {
        return dp[n];
    }

    /* Compute any missing values (recurrence applies for indices ≥ 2). */
    idx_t start = (dp_max < 2) ? 2 : dp_max + 1;
    for (idx_t k = start; k <= n; ++k) {
        idx_t pow = highest_power_of_two(k);
        idx_t j   = k - pow;          /* 0 ≤ j < pow */
        dp[k] = ((seq_t)3 * dp[j]) + dp[j + 1];
    }
    dp_max = n;
    return dp[n];
}
