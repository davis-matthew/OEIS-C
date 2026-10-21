/*
 * A061423  Sum of digits = 6 times number of digits.
 * https://oeis.org/A061423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   numbers whose sum of decimal digits equals six times the number
 *            of digits
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(n) returns the nth such number in increasing order. */

#include <stdint.h>
#include <stdbool.h>

#define MAX_SUM 2000          /* enough for all lengths needed up to n≈10000 */

/* number of digit sequences of length d (allowing leading zeros) whose digits sum to sum */
static uint64_t count_solutions(unsigned int d, unsigned int sum)
{
    if (sum > 9u * d) return 0;
    static uint64_t dp[MAX_SUM + 1];
    static uint64_t newdp[MAX_SUM + 1];
    for (unsigned int i = 0; i <= sum; ++i) dp[i] = 0;
    dp[0] = 1;
    for (unsigned int pos = 0; pos < d; ++pos) {
        for (unsigned int i = 0; i <= sum; ++i) newdp[i] = 0;
        for (unsigned int s = 0; s <= sum; ++s) {
            uint64_t cur = dp[s];
            if (cur == 0) continue;
            for (int dig = 0; dig <= 9; ++dig) {
                unsigned int ns = s + dig;
                if (ns > sum) break;
                newdp[ns] += cur;
            }
        }
        for (unsigned int i = 0; i <= sum; ++i) dp[i] = newdp[i];
    }
    return dp[sum];
}

/* construct the offset‑th (0‑based) valid number of length d with digit sum sum */
static seq_t build_nth_in_length(unsigned int d, unsigned int sum,
                                 idx_t offset)
{
    seq_t result = 0;
    idx_t remOffset = offset;
    unsigned int remDigits = d;
    unsigned int remSum = sum;
    for (unsigned int pos = 0; pos < d; ++pos) {
        int minDigit = (pos == 0) ? 1 : 0;
        for (int dig = minDigit; dig <= 9; ++dig) {
            if ((unsigned int)dig > remSum) break;
            unsigned int remaining = remDigits - 1;
            unsigned int newSum = remSum - dig;
            uint64_t cnt = count_solutions(remaining, newSum);
            if (remOffset < cnt) {
                result = result * 10 + (seq_t)dig;
                remDigits = remaining;
                remSum = newSum;
                break;
            } else {
                remOffset -= cnt;
            }
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t target = n;
    idx_t cumulative = 0;
    unsigned int d = 1;
    while (1) {
        unsigned int sum = 6u * d;
        uint64_t total   = count_solutions(d, sum);
        uint64_t leading = count_solutions(d - 1, sum);
        uint64_t valid   = total - leading;
        if (cumulative + valid >= target) {
            idx_t offset = target - cumulative - 1;
            return build_nth_in_length(d, sum, offset);
        }
        cumulative += valid;
        ++d;
    }
}
