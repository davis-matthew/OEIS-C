/*
 * A161518  Number of reduced words of length n in the Weyl group A_21.
 * https://oeis.org/A161518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{21} (1+x+...+x^k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 15, u64 n <= 101, u128 n <= 231,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_N 231

seq_t a(idx_t n)
{
    static seq_t coeff[MAX_N + 1];
    static bool initialized = false;

    if (!initialized) {
        seq_t dp[MAX_N + 1];
        seq_t newdp[MAX_N + 1];

        for (idx_t i = 0; i <= MAX_N; ++i) dp[i] = 0;
        dp[0] = 1;

        for (idx_t k = 1; k <= 21; ++k) {
            for (idx_t i = 0; i <= MAX_N; ++i) newdp[i] = 0;

            for (idx_t j = 0; j <= MAX_N; ++j) {
                seq_t val = dp[j];
                if (val == 0) continue;
                for (idx_t t = 0; t <= k && j + t <= MAX_N; ++t) {
                    newdp[j + t] += val;
                }
            }

            for (idx_t i = 0; i <= MAX_N; ++i) dp[i] = newdp[i];
        }

        for (idx_t i = 0; i <= MAX_N; ++i) coeff[i] = dp[i];
        initialized = true;
    }

    if (n > MAX_N) return (seq_t)0;
    return coeff[n];
}
