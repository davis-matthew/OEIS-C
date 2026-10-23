/*
 * A034148  Number of partitions of n into distinct parts from [1, 18].
 * https://oeis.org/A034148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of (1+x)(1+x^2)…(1+x^18) in the expansion of
 *            x^n
 * Range:     u8 n <= 30, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Max sum of distinct parts 1..18 is 171; for n>171 the value is 0. */
    static seq_t cache[172];
    static bool initialized = false;

    if (!initialized) {
        unsigned long long dp[172] = {0};
        dp[0] = 1ULL;
        for (int part = 1; part <= 18; ++part) {
            for (int sum = 171; sum >= part; --sum) {
                dp[sum] += dp[sum - part];
            }
        }
        for (int i = 0; i < 172; ++i) {
            cache[i] = (seq_t)dp[i];
        }
        initialized = true;
    }

    if (n < 172u) {
        return cache[(size_t)n];
    } else {
        return (seq_t)0;
    }
}
