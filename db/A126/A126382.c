/*
 * A126382  Number of base 28 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of 28‑base n‑digit strings with adjacent digits differing by at most 1.
    Computation: dynamic programming on a 28‑node path graph.
    Start with one‑digit sequences (dp[i] = 1 for all i).
    For each additional digit, update:
        next[i] = cur[i] + cur[i-1] + cur[i+1]   (indices within [0,27]).
    Finally sum all dp[i] to get a(n).  a(0) = 1, a(1) = 28. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t cur[28];
    seq_t nxt[28];

    /* initial one‑digit sequences */
    for (idx_t i = 0; i < 28; ++i)
        cur[i] = 1;

    /* extend sequences up to length n */
    for (idx_t step = 1; step < n; ++step) {
        for (idx_t i = 0; i < 28; ++i) {
            seq_t val = cur[i];
            if (i > 0)
                val += cur[i - 1];
            if (i + 1 < 28)
                val += cur[i + 1];
            nxt[i] = val;
        }
        for (idx_t i = 0; i < 28; ++i)
            cur[i] = nxt[i];
    }

    /* total number of n‑digit strings */
    seq_t total = 0;
    for (idx_t i = 0; i < 28; ++i)
        total += cur[i];

    return total;
}
