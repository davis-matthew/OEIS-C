/*
 * A061387  Sum of digits = 4 times number of digits.
 * https://oeis.org/A061387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coeff: numbers such that digit_sum(n) = 4 * number_of_digits(n).
   The function uses combinatorial counting to locate the nth such
   integer directly, rather than brute‑forcing each candidate. */
#include <stdint.h>

#define MAX_LEN 60            /* should be enough for n <= 10000 */
static uint64_t dp[MAX_LEN+1][MAX_LEN*9+1];
static int dp_ready = 0;

static void init_dp(void)
{
    if (dp_ready) return;
    dp_ready = 1;
    for (int len = 0; len <= MAX_LEN; ++len)
        for (int sum = 0; sum <= 9*len; ++sum)
            dp[len][sum] = 0;
    dp[0][0] = 1ULL;
    for (int len = 1; len <= MAX_LEN; ++len) {
        for (int sum = 0; sum <= 9*len; ++sum) {
            uint64_t total = 0;
            for (int d = 0; d <= 9; ++d) {
                if (sum < d) break;
                total += dp[len-1][sum-d];
                if (total > (uint64_t)5e18) total = (uint64_t)5e18; /* cap */
            }
            dp[len][sum] = total;
        }
    }
}

static uint64_t count_len_sum(int len, int sum)
{
    if (len <= 0) return (sum == 0) ? 1ULL : 0ULL;
    uint64_t total = 0;
    for (int first = 1; first <= 9; ++first) {
        int rem = sum - first;
        if (rem < 0) continue;
        if (rem > 9*(len-1)) continue;
        total += dp[len-1][rem];
        if (total > (uint64_t)5e18) total = (uint64_t)5e18; /* cap */
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    init_dp();

    idx_t target = n;
    for (int len = 1; len <= MAX_LEN; ++len) {
        uint64_t cnt = count_len_sum(len, 4*len);
        if (target > cnt) {
            target -= cnt;
            continue;
        }

        seq_t res = 0;
        int remainingSum = 4*len;

        for (int pos = 0; pos < len; ++pos) {
            int start = (pos == 0) ? 1 : 0;
            for (int digit = start; digit <= 9; ++digit) {
                int rem = remainingSum - digit;
                if (rem < 0) break;
                int remLen = len - 1 - pos;
                if (rem > 9*remLen) continue;
                uint64_t ways = dp[remLen][rem];
                if (target > ways) {
                    target -= ways;
                    continue;
                }
                res = res * (seq_t)10 + (seq_t)digit;
                remainingSum = rem;
                break;
            }
        }
        return res;
    }

    /* Fallback for very large n – extend length beyond MAX_LEN */
    int len = MAX_LEN + 1;
    while (1) {
        uint64_t cnt = count_len_sum(len, 4*len);
        if (target <= cnt) {
            seq_t res = 0;
            int remainingSum = 4*len;
            for (int pos = 0; pos < len; ++pos) {
                int start = (pos == 0) ? 1 : 0;
                for (int digit = start; digit <= 9; ++digit) {
                    int rem = remainingSum - digit;
                    if (rem < 0) break;
                    int remLen = len - 1 - pos;
                    if (rem > 9*remLen) continue;
                    uint64_t ways = dp[remLen][rem];
                    if (target > ways) {
                        target -= ways;
                        continue;
                    }
                    res = res * (seq_t)10 + (seq_t)digit;
                    remainingSum = rem;
                    break;
                }
            }
            return res;
        } else {
            target -= cnt;
            ++len;
            if (len > 120) break; /* safety guard; not expected for n <= 10 000 */
        }
    }

    return (seq_t)0; /* should never reach here for valid n */
}
