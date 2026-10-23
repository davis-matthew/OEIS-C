/*
 * A029004  Expansion of 1/((1-x)(1-x^2)(1-x^3)(1-x^10)).
 * https://oeis.org/A029004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 1, 2, 3, and 10. We
 *            compute it with a simple dynamic‑programming recurrence using
 *            the generating function 1/((1−x)(1−x²)(1−x³)(1−x¹⁰)). For each
 *            part size p, we add the number of partitions of n−p
 * Range:     u8 n <= 37, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate DP array of size n+1 (seq_t is unsigned) */
    idx_t   size = n + 1;
    seq_t *dp   = (seq_t *)calloc((size_t)size, sizeof(seq_t));
    if (!dp)      /* allocation failed – terminate safely */
    {
        /* In a contest setting you would handle this differently,
           but for correctness we simply abort. */
        abort();
    }

    /* base case: one way to partition 0 (use no parts). */
    dp[0] = (seq_t)1;

    /* part sizes allowed in this sequence */
    const idx_t parts[4] = {1, 2, 3, 10};

    /* run the DP: for each part size, add to all larger indices */
    for (int pi = 0; pi < 4; ++pi)
    {
        idx_t part = parts[pi];
        if (part > n) continue;      /* skip too large parts */
        for (idx_t i = part; i <= n; ++i)
        {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
