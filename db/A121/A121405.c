/*
 * A121405  Numbers that are not the sum of distinct pentagonal numbers
 *          (A000326).
 * https://oeis.org/A121405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) = n-th positive integer that is NOT the sum of distinct
 *            pentagonal numbers (P(k) = k(3k-1)/2). The value is computed by
 *            incrementally building a subset‑sum dynamic‑program over all
 *            pentagonal numbers up to the current test value
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Static state used by the function. */
static bool            initialized   = false;
static idx_t           next_x        = 1;          /* Next integer to test for missingness. */
static idx_t           next_k        = 1;          /* Next k for pentagonal number. */
static idx_t           next_p        = 1;          /* Value of P(next_k). */
static uint8_t        *dp            = NULL;        /* Bitset: dp[i] == 1 if i is representable. */
static size_t          dp_cap        = 0;          /* Allocated size of dp array. */

static seq_t          *missing       = NULL;        /* Stored missing numbers (in order). */
static size_t          miss_cap      = 0;
static size_t          miss_len      = 0;

/* Grow the dp array to contain at least indices 0..new_cap-1. */
static void ensure_dp(idx_t new_cap) {
    if (new_cap <= dp_cap) return;
    uint8_t *new_dp = (uint8_t *)malloc(new_cap);
    if (!new_dp) abort();
    if (dp) {
        memcpy(new_dp, dp, dp_cap);
        free(dp);
    }
    memset(new_dp + dp_cap, 0, new_cap - dp_cap);
    dp = new_dp;
    dp_cap = new_cap;
}

/* Add a new coin of value p to the subset‑sum DP. */
static void add_coin(idx_t p) {
    /* Ensure dp array can hold all new sums. */
    size_t old_cap = dp_cap;
    size_t new_cap = dp_cap + p;          /* old_cap + p because i+p may reach old_cap-1 + p */
    ensure_dp(new_cap);

    /* For each existing representable i, mark i+p as representable. */
    for (idx_t i = old_cap - 1; ; --i) {
        if (dp[i]) dp[i + p] = 1;
        if (i == 0) break;
    }
}

/* Grow the missing array to hold one more element. */
static void grow_missing(void) {
    size_t new_cap = miss_cap ? miss_cap * 2 : 4;
    seq_t *new_mem = (seq_t *)realloc(missing, new_cap * sizeof(seq_t));
    if (!new_mem) abort();
    missing = new_mem;
    miss_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n=0 is outside the OEIS offset; return 0. */

    /* One‑time initialization. */
    if (!initialized) {
        ensure_dp(1);              /* Allocate dp[0]. */
        dp[0] = 1;                 /* Zero is always representable. */
        initialized = true;
    }

    /* Generate missing numbers until we have produced the n‑th one. */
    while (miss_len < (size_t)n) {
        /* Expand the DP to at least next_x. */
        ensure_dp(next_x + 1);

        /* Add all pentagonal numbers that fit into the current test value. */
        while (next_p <= next_x) {
            add_coin(next_p);
            ++next_k;
            next_p = next_k * (3 * next_k - 1) / 2;
        }

        /* If next_x is not representable, store it. */
        if (!dp[next_x]) {
            if (miss_len == miss_cap) grow_missing();
            missing[miss_len++] = (seq_t)next_x;
        }
        ++next_x;
    }

    return missing[n - 1];
}
