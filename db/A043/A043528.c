/*
 * A043528  Numbers having four 9's in base 10.
 * https://oeis.org/A043528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th (positive) integer whose decimal representation
 *            contains exactly four digits equal to 9
 * Range:     u8 none, u16 n <= 6, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 47 terms through n = 9069 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

/* Count of integers x (0 <= x <= limit) that have exactly four 9's in base 10.
   The count is saturated at limit+1 to avoid overflow during binary search. */
static idx_t count_4nines_limit(uint64_t limitX, idx_t target)
{
    /* limit for saturation */
    const idx_t LIMIT = (target == UINT64_MAX ? UINT64_MAX : target + 1);

    /* extract decimal digits of limitX (most significant first) */
    int digits[20];
    int len = 0;
    uint64_t tmp = limitX;
    do {
        digits[len++] = tmp % 10;
        tmp /= 10;
    } while (tmp);
    /* reverse to most significant first */
    for (int i = 0; i < len / 2; ++i) {
        int t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* DP[j][tight] : number of ways so far with j nines and tight flag.
       We only need j = 0..4. */
    idx_t dp[5][2] = {0};
    dp[0][1] = 1;   /* start with zero digits, tight = true */

    for (int pos = 0; pos < len; ++pos) {
        idx_t ndp[5][2] = {0};
        int max_digit = 9;
        for (int j = 0; j <= 4; ++j) {
            for (int tight = 0; tight <= 1; ++tight) {
                idx_t cur = dp[j][tight];
                if (!cur) continue;
                if (!tight) max_digit = 9;
                else max_digit = digits[pos];
                for (int d = 0; d <= max_digit; ++d) {
                    int new_j = j + (d == 9 ? 1 : 0);
                    if (new_j > 4) continue;
                    int new_tight = (tight && d == digits[pos]) ? 1 : 0;
                    idx_t val = ndp[new_j][new_tight] + cur;
                    if (val >= LIMIT) val = LIMIT;
                    ndp[new_j][new_tight] = val;
                }
            }
        }
        /* copy back */
        for (int j = 0; j <= 4; ++j) {
            dp[j][0] = ndp[j][0];
            dp[j][1] = ndp[j][1];
        }
    }

    /* total count with exactly four 9's */
    idx_t res = dp[4][0] + dp[4][1];
    if (res >= LIMIT) res = LIMIT;
    return res;
}

seq_t a(idx_t n)
{
    /* Find a high bound that is guaranteed to contain at least n numbers. */
    uint64_t high = 1;
    while (count_4nines_limit(high, n) <= n) {
        if (high > UINT64_MAX / 2) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    uint64_t low = 0;
    while (low < high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (count_4nines_limit(mid, n) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
