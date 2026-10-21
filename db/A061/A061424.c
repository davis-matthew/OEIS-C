/*
 * A061424  Sum of digits = 7 times number of digits.
 * https://oeis.org/A061424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th positive integer whose digit sum = 7 * (number
 *            of digits)
 * Range:     u8 n <= 6, u16 n <= 414, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _init_dp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_init_dp.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0 */

    /* Pre‑compute DP tables for up to 25 digits (more than enough for n ≤ 10⁴) */
    enum { MAX_D = 25 };
    static unsigned long long dp[MAX_D + 1][MAX_D * 7 + 1];
    static int dp_initialized = 0;
    if (!dp_initialized) {
        _init_dp(dp, MAX_D, MAX_D * 7);
        dp_initialized = 1;
    }

    /* Find the digit length of the n‑th term */
    unsigned long long cum = 0;          /* cumulative count of found terms */
    int d = 1;                           /* current number of digits */
    unsigned long long idx = n;          /* 1‑based index within current length */

    for (;;) {
        unsigned long long cnt = 0;      /* number of d‑digit numbers satisfying the condition */
        for (int first = 1; first <= 9; ++first) {
            int rest_sum = 7 * d - first;
            if (rest_sum < 0 || rest_sum > (d - 1) * 9) continue;
            cnt += dp[d - 1][rest_sum];
        }
        if (cum + cnt >= idx) {
            idx -= cum;                  /* re‑index within this digit length */
            break;
        }
        cum += cnt;
        ++d;
    }

    /* Construct the idx‑th number of length d digit by digit (lexicographic order) */
    seq_t val = 0;
    int sum_needed = 7 * d;              /* total sum still required */

    for (int pos = 0; pos < d; ++pos) {
        int digit_start = (pos == 0) ? 1 : 0;
        int digit_end   = 9;
        for (int digit = digit_start; digit <= digit_end; ++digit) {
            int rem_sum = sum_needed - digit;
            int rem_digits = d - pos - 1;

            unsigned long long ways = 0;
            if (rem_digits == 0) {
                ways = (rem_sum == 0) ? 1ULL : 0ULL;
            } else {
                if (rem_sum >= 0 && rem_sum <= rem_digits * 9)
                    ways = dp[rem_digits][rem_sum];
                else
                    ways = 0;
            }

            if (ways >= idx) {
                val = val * 10 + digit;
                sum_needed = rem_sum;
                break;
            } else {
                idx -= ways;
            }
        }
    }

    return val;
}
