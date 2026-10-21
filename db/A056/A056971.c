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
 * Helpers:   binom, left_size
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/binom.h"
#include "../utils/left_size.h"

static seq_t *dp = NULL;          /* memoisation array */
static idx_t dp_alloc = 0;       /* size of allocated array (slots) */
static idx_t dp_comp = 0;        /* largest index computed so far */

/* Ensure dp has room for indices up to n (inclusive) */
static void ensure_size(idx_t n)
{
    if (dp_alloc > n)
        return;                   /* already large enough */

    size_t new_size = (size_t)n + 1;   /* number of slots needed */
    seq_t *new_arr = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_arr)
        return; /* allocation failed – unlikely in the build environment */

    /* copy existing values */
    if (dp) {
        memcpy(new_arr, dp, dp_alloc * sizeof(seq_t));
        free(dp);
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
