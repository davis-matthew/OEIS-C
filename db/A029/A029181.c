/*
 * A029181  Expansion of 1/((1-x^2)*(1-x^4)*(1-x^7)*(1-x^8)).
 * https://oeis.org/A029181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming for partitions into parts 2,4,7,8. */

#include <stddef.h>   /* for size_t, SIZE_MAX */
#include <stdlib.h>   /* for calloc, free (C) */
#ifdef __cplusplus
#include <new>        /* for operator new/delete */
#endif

/* The function defined by the problem statement. */
seq_t a(idx_t n)
{
    /* Make sure the requested size fits in size_t for array allocation. */
    if (n > (idx_t)SIZE_MAX) {
        return (seq_t)0;   /* out of bounds: return 0 as a safe default */
    }

    size_t len = (size_t)n + 1;  /* array length: we need indices 0..n */

    seq_t *dp = NULL;
#ifdef __cplusplus
    /* In C++ use new/delete to construct an array of seq_t objects. */
    try {
        dp = new seq_t[len];
    } catch (...) {
        return (seq_t)0;   /* allocation failed */
    }
#else
    /* In C use calloc which zero-initialises the memory. */
    dp = (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp) {
        return (seq_t)0;   /* allocation failed */
    }
#endif

    /* Base case: one way to write 0. */
    dp[0] = (seq_t)1;

    /* The allowed part sizes. */
    static const idx_t parts[4] = {2, 4, 7, 8};

    for (int pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        if (p > n) continue;           /* parts larger than n are useless   */
        size_t sp = (size_t)p;          /* convert to size_t for indexing   */

        for (size_t i = sp; i < len; ++i) {
            dp[i] += dp[i - sp];
        }
    }

    seq_t result = dp[(size_t)n];

#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif

    return result;
}
