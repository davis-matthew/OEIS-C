/*
 * A056971  Number of (binary) heaps on n elements.
 * https://oeis.org/A056971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(n-1, L) * a(L) * a(R) where L is the size of the
 *            left subtree of a complete binary heap with n nodes. The size is
 *            computed as h = floor(log2(n)) h1 = h-1 nodes_before = 2^h - 1
 *            last_level = n - nodes_before left_last = min(last_level, 2^h1)
 *            L = 2^h1 - 1 + left_last and R = n-1 - L. T...
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 17, u64 n <= 27, u128 n <= 44,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 479 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute binomial coefficient C(a,b) using multiplicative formula. */
static seq_t binom(idx_t a, idx_t b) {
    if (b > a) return 0;
    if (b > a - b) b = a - b;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res *= (seq_t)(a - b + i);
        res /= (seq_t)i;
    }
    return res;
}

/* size of left subtree in a complete binary heap of n nodes */
static idx_t left_size(idx_t n)
{
    if (n <= 1)
        return 0;

    /* compute h = floor(log2(n)) */
    idx_t h = 0;
    idx_t temp = n;
    while (temp > 1) {
        temp >>= 1;
        ++h;
    }

    /* number of nodes before the last level */
    idx_t nodes_before = ((idx_t)1 << h) - 1;

    /* nodes in the last level */
    idx_t last = n - nodes_before;

    /* size of last level of left subtree (max 2^(h-1)) */
    idx_t max_left_last = ((idx_t)1 << (h - 1));
    idx_t left_last = last;
    if (left_last > max_left_last)
        left_last = max_left_last;

    /* total nodes in left subtree */
    return ((idx_t)1 << (h - 1)) - 1 + left_last;
}

static seq_t *dp = NULL;          /* memoisation array */
static idx_t dp_alloc = 0;       /* size of allocated array (slots) */
static idx_t dp_comp = 0;        /* largest index computed so far */

/* Ensure dp has room for indices up to n (inclusive) */
static void ensure_size(idx_t n)
{
    if (dp_alloc > n)
        return;                   /* already large enough */

    size_t new_size = (size_t)n + 1;   /* number of slots needed */
    seq_t *new_arr = mem_new(seq_t, new_size);
    if (!new_arr)
        return; /* allocation failed – unlikely in the build environment */

    /* copy existing values */
    if (dp) {
        mem_copy(new_arr, dp, dp_alloc * sizeof(seq_t));
        mem_free(dp);
    }

    /* initialise any new slots to 0 – they will be overwritten */
    for (idx_t i = dp_alloc; i < new_size; ++i)
        new_arr[i] = 0;

    dp = new_arr;
    dp_alloc = (idx_t)new_size;
}

/* Compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* lazy initialisation of dp for the first call */
    if (dp == NULL) {
        ensure_size(2);        /* create slots for indices 0,1,2 */
        dp[0] = (seq_t)1;
        dp[1] = (seq_t)1;
        dp[2] = (seq_t)1;
        dp_comp = 2;
    }

    if (n <= 2)
        return (seq_t)1;

    /* make sure the array can hold up to index n */
    ensure_size(n);

    /* compute any missing entries up to n */
    for (idx_t i = dp_comp + 1; i <= n; ++i) {
        idx_t left  = left_size(i);
        idx_t right = i - 1 - left;
        seq_t bin   = binom(i - 1, left);     /* choose elements for left subtree */
        dp[i] = bin * dp[left] * dp[right];
    }

    dp_comp = n;
    return dp[n];
}
