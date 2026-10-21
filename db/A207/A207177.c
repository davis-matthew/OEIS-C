/*
 * A207177  Number of n X 3 0..1 arrays avoiding 0 0 0 horizontally and 0 1 0
 *          vertically.
 * https://oeis.org/A207177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n X 3 0..1 arrays avoiding 0 0 0 horizontally
 *            and 0 1 0 vertically. Computation via DP over rows; initial
 *            values a(1)=7, a(2)=49
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset is 1, but handle n==0 safely */
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)49;

    /* DP state: last two rows encoded as 8*row1 + row2 (row values 1..7). */
    seq_t dp[64]      = {0};  /* current DP */
    seq_t dp_next[64] = {0};  /* next DP */

    /* Initialize for n = 2: all pairs of non‑zero rows are admissible. */
    for (int a = 1; a < 8; ++a)
        for (int b = 1; b < 8; ++b)
            dp[(a << 3) | b] = 1;

    /* Add rows 3 .. n. */
    for (idx_t row = 3; row <= n; ++row)
    {
        /* clear dp_next */
        for (int i = 0; i < 64; ++i) dp_next[i] = 0;

        /* transition from every state (a,b) adding new row c */
        for (int a = 1; a < 8; ++a)
            for (int b = 1; b < 8; ++b)
            {
                seq_t ways = dp[(a << 3) | b];
                if (!ways) continue;

                for (int c = 1; c < 8; ++c)
                {
                    bool ok = true;
                    /* check vertical 0 1 0 in each column */
                    for (int j = 0; j < 3 && ok; ++j)
                    {
                        int top = (a >> j) & 1;
                        int mid = (b >> j) & 1;
                        int bot = (c >> j) & 1;
                        if (top == 0 && mid == 1 && bot == 0) ok = false;
                    }
                    if (ok) dp_next[(b << 3) | c] += ways;
                }
            }

        /* move dp_next to dp */
        for (int i = 0; i < 64; ++i) dp[i] = dp_next[i];
    }

    /* sum all ending states */
    seq_t total = 0;
    for (int a = 1; a < 8; ++a)
        for (int b = 1; b < 8; ++b)
            total += dp[(a << 3) | b];

    return total;
}
