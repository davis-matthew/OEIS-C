/*
 * A144116  Number of non-Fibonacci parts in all partitions of n.
 * https://oeis.org/A144116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006128(n) - A144115(n) (total number of parts in all
 *            partitions of n minus the number of Fibonacci parts in all
 *            partitions)
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 110, u64 n <= 369,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Computes a(n), number of non‑Fibonacci parts in all partitions of n.
   Returns a value of type seq_t; idx_t is an unsigned integer type. */
seq_t a(idx_t n)
{
    /* Arrays of size n+1, indexed 0..n. All entries are zero‐initialized. */
    seq_t *dp     = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t)); /* #partitions */
    seq_t *parts  = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t)); /* total parts */
    seq_t *fparts = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t)); /* fib parts */
    bool  *isFib  = (bool*)calloc((size_t)n + 1, sizeof(bool));   /* Fibonacci flag */

    if (!dp || !parts || !fparts || !isFib)
    {
        free(dp); free(parts); free(fparts); free(isFib);
        return 0;
    }

    /* Mark Fibonacci numbers up to n. */
    if (n >= 1) isFib[(size_t)1] = true;
    if (n >= 2) isFib[(size_t)2] = true;
    uint64_t f1 = 1, f2 = 1;
    while (true)
    {
        uint64_t f = f1 + f2;
        if (f > (uint64_t)n) break;
        isFib[(size_t)f] = true;
        f1 = f2; f2 = f;
    }

    /* Base case for partition of 0 */
    dp[0] = 1;

    /* Dynamic programming over part sizes i = 1 .. n */
    for (idx_t i = 1; i <= n; ++i)
    {
        bool fib = isFib[(size_t)i];
        for (idx_t j = i; j <= n; ++j)
        {
            size_t prev = (size_t)(j - i);
            seq_t add  = dp[prev];

            dp[(size_t)j]     += add;
            parts[(size_t)j]  += parts[prev] + add;
            if (fib)
                fparts[(size_t)j] += fparts[prev] + add;
            else
                fparts[(size_t)j] += fparts[prev];
        }
    }

    seq_t result = parts[(size_t)n] - fparts[(size_t)n];

    /* Clean up */
    free(dp); free(parts); free(fparts); free(isFib);
    return result;
}
