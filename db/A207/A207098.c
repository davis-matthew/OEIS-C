/*
 * A207098  Number of 0..6 arrays x(0..n-1) of n elements with each no smaller
 *          than the sum of its two previous neighbors modulo 7.
 * https://oeis.org/A207098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=7, a(2)=28; For n≥3, let dp[a,b] be the number of arrays of
 *            the current length ending with the last two values a,b
 *            (0≤a,b≤6). Initially (length 2) dp[a,b]=1 if b≥a else 0. For
 *            each new element c, the transition is dp_next[b,c] += dp[a,b]
 *            for all c ≥ (a+b) mod 7. After processing all elements...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)7;
    if (n == 0) return (seq_t)0;  /* optional, not used by OEIS offset */

    const int M = 49;              /* 7 × 7 possible pairs (a,b) */
    seq_t dp_prev[M];
    seq_t dp_next[M];

    /* ------------------------------------------------------------------ */
    /* initialize dp_prev for length 2: only pairs with b≥a are allowed     */
    /* ------------------------------------------------------------------ */
    for (int i = 0; i < M; ++i) dp_prev[i] = (seq_t)0;
    for (int a = 0; a < 7; ++a)
        for (int b = a; b < 7; ++b)
            dp_prev[a * 7 + b] = (seq_t)1;

    if (n == 2) {
        seq_t total = (seq_t)0;
        for (int i = 0; i < M; ++i) total += dp_prev[i];
        return total;
    }

    /* ------------------------------------------------------------------ */
    /* extend the array one element at a time for lengths 3 … n            */
    /* ------------------------------------------------------------------ */
    for (idx_t len = 3; len <= n; ++len) {
        for (int i = 0; i < M; ++i) dp_next[i] = (seq_t)0;

        for (int idx = 0; idx < M; ++idx) {
            seq_t val = dp_prev[idx];
            if (val == (seq_t)0) continue;

            int a = idx / 7;
            int b = idx % 7;
            int lower = (a + b) % 7;           /* minimum allowed value for c */

            for (int c = lower; c <= 6; ++c) {
                int j = b * 7 + c;
                dp_next[j] += val;
            }
        }

        /* move to the next length */
        for (int i = 0; i < M; ++i) dp_prev[i] = dp_next[i];
    }

    /* sum all states to obtain a(n) */
    seq_t total = (seq_t)0;
    for (int i = 0; i < M; ++i) total += dp_prev[i];
    return total;
}
