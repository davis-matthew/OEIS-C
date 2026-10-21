/*
 * A224187  Number of nX5 0..2 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A224187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 arrays with entries 0..2, rows unimodal
 *            (nondecreasing then nonincreasing), columns nondecreasing.
 *            Computed via dynamic programming over the 5‑dimensional poset of
 *            states
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 164, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 3^5 = 243 possible column vectors (states).  */
    static bool unimodal[243];
    static int unimodal_init = 0;

    /* Precompute which 5‑tuples are unimodal. */
    if (!unimodal_init) {
        for (int a = 0; a < 3; ++a)
            for (int b = 0; b < 3; ++b)
                for (int c = 0; c < 3; ++c)
                    for (int d = 0; d < 3; ++d)
                        for (int e = 0; e < 3; ++e) {
                            int digit[5] = {a, b, c, d, e};
                            bool ok = false;
                            for (int peak = 0; peak < 5; ++peak) {
                                bool inc = true, dec = true;
                                for (int i = 0; i < peak; ++i)
                                    if (digit[i] > digit[i + 1]) inc = false;
                                for (int i = peak; i < 4; ++i)
                                    if (digit[i] < digit[i + 1]) dec = false;
                                if (inc && dec) { ok = true; break; }
                            }
                            int idx = a * 81 + b * 27 + c * 9 + d * 3 + e;
                            unimodal[idx] = ok;
                        }
        unimodal_init = 1;
    }

    /* DP array: number of sequences ending at each state. */
    seq_t dp[243];
    for (int idx = 0; idx < 243; ++idx)
        dp[idx] = unimodal[idx] ? (seq_t)1 : (seq_t)0;

    /* For n = 1 the answer is already ready. */
    if (n == 1) {
        seq_t sum = 0;
        for (int idx = 0; idx < 243; ++idx) sum += dp[idx];
        return sum;
    }

    /* Iterate for rows 2 .. n.  */
    for (idx_t row = 2; row <= n; ++row) {
        /* Cumulative sums along dimension a (first digit). */
        for (int a = 1; a < 3; ++a)
            for (int b = 0; b < 3; ++b)
                for (int c = 0; c < 3; ++c)
                    for (int d = 0; d < 3; ++d)
                        for (int e = 0; e < 3; ++e) {
                            int idx  = a * 81 + b * 27 + c * 9 + d * 3 + e;
                            int prev = (a - 1) * 81 + b * 27 + c * 9 + d * 3 + e;
                            dp[idx] += dp[prev];
                        }

        /* Dimension b */
        for (int a = 0; a < 3; ++a)
            for (int b = 1; b < 3; ++b)
                for (int c = 0; c < 3; ++c)
                    for (int d = 0; d < 3; ++d)
                        for (int e = 0; e < 3; ++e) {
                            int idx  = a * 81 + b * 27 + c * 9 + d * 3 + e;
                            int prev = a * 81 + (b - 1) * 27 + c * 9 + d * 3 + e;
                            dp[idx] += dp[prev];
                        }

        /* Dimension c */
        for (int a = 0; a < 3; ++a)
            for (int b = 0; b < 3; ++b)
                for (int c = 1; c < 3; ++c)
                    for (int d = 0; d < 3; ++d)
                        for (int e = 0; e < 3; ++e) {
                            int idx  = a * 81 + b * 27 + c * 9 + d * 3 + e;
                            int prev = a * 81 + b * 27 + (c - 1) * 9 + d * 3 + e;
                            dp[idx] += dp[prev];
                        }

        /* Dimension d */
        for (int a = 0; a < 3; ++a)
            for (int b = 0; b < 3; ++b)
                for (int c = 0; c < 3; ++c)
                    for (int d = 1; d < 3; ++d)
                        for (int e = 0; e < 3; ++e) {
                            int idx  = a * 81 + b * 27 + c * 9 + d * 3 + e;
                            int prev = a * 81 + b * 27 + c * 9 + (d - 1) * 3 + e;
                            dp[idx] += dp[prev];
                        }

        /* Dimension e */
        for (int a = 0; a < 3; ++a)
            for (int b = 0; b < 3; ++b)
                for (int c = 0; c < 3; ++c)
                    for (int d = 0; d < 3; ++d)
                        for (int e = 1; e < 3; ++e) {
                            int idx  = a * 81 + b * 27 + c * 9 + d * 3 + e;
                            int prev = a * 81 + b * 27 + c * 9 + d * 3 + (e - 1);
                            dp[idx] += dp[prev];
                        }

        /* Zero out non‑unimodal states – they cannot be a valid row. */
        for (int idx = 0; idx < 243; ++idx)
            if (!unimodal[idx]) dp[idx] = 0;
    }

    /* Sum all counts – any ending state is allowed. */
    seq_t result = 0;
    for (int idx = 0; idx < 243; ++idx)
        result += dp[idx];
    return result;
}
