/*
 * A039575  Numbers whose base-6 representation has the same number of 0's,
 *          1's and 4's.
 * https://oeis.org/A039575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts integers whose base‑6 representation contains an equal
 * number of the digits 0, 1, and 4.  The function a(n) returns the n‑th
 * such integer (offset 1).  The value is found by binary searching the
 * integers and using a digit‑DP to count how many integers ≤ limit meet the
 * condition.  The DP tracks the differences
 *   diff01 = (#0 – #1)   and   diff04 = (#0 – #4)
 * while also respecting the tightness of the bound and whether a non‑zero
 * digit has already appeared.  */
 
#include <stdint.h>
#include <stdbool.h>
 
/* Count how many integers from 0 to limit (inclusive) have equal numbers of
 * digits 0, 1 and 4 in their base-6 representation.  The count does not
 * include 0 itself (which does not satisfy the property).  */
static unsigned long long count_le(uint64_t limit)
{
    if (limit < 2) return 0;
    /* Convert limit to base‑6 digits (most significant first). */
    uint8_t digits[64];
    int len = 0;
    uint64_t tmp = limit;
    while (tmp) {
        digits[len++] = tmp % 6;
        tmp /= 6;
    }
    /* Now digits[0] … digits[len-1] are in reverse order.
       Reverse them to get most significant first. */
    for (int i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }
 
    /* DP arrays: dp[diff01][diff04][tight][started] */
    int offset = len;                   /* shift to handle negative diffs */
    int sz = 2 * offset + 1;
    /* Allocate on stack: 41*41*2*2 ≈ 6724 entries maximum. */
    unsigned long long dp[41][41][2][2] = {0};
    unsigned long long ndp[41][41][2][2] = {0};
    dp[offset][offset][1][0] = 1;        /* start before first digit, tight=1, started=0 */
 
    for (int pos = 0; pos < len; ++pos) {
        /* Clear ndp */
        for (int i = 0; i < sz; ++i)
            for (int j = 0; j < sz; ++j)
                ndp[i][j][0][0] = ndp[i][j][0][1] =
                ndp[i][j][1][0] = ndp[i][j][1][1] = 0;
 
        uint8_t maxd = (dp[0][0][1][0] || dp[0][0][1][1] /* any state with tight=1 */ ? digits[pos] : 5);
        for (int d01 = 0; d01 < sz; ++d01) {
            for (int d04 = 0; d04 < sz; ++d04) {
                for (int tight = 0; tight < 2; ++tight) {
                    for (int started = 0; started < 2; ++started) {
                        unsigned long long cur = dp[d01][d04][tight][started];
                        if (!cur) continue;
                        uint8_t limitDigit = tight ? digits[pos] : 5;
                        for (uint8_t d = 0; d <= limitDigit; ++d) {
                            bool ntight = tight && (d == limitDigit);
                            bool nstarted = started || (d != 0);
                            int nd01 = d01;
                            int nd04 = d04;
                            if (nstarted) {
                                if (d == 0) { nd01++; nd04++; }
                                else if (d == 1) { nd01--; }
                                else if (d == 4) { nd04--; }
                            }
                            ndp[nd01][nd04][ntight][nstarted] += cur;
                        }
                    }
                }
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < sz; ++i)
            for (int j = 0; j < sz; ++j)
                for (int t = 0; t < 2; ++t)
                    for (int s = 0; s < 2; ++s)
                        dp[i][j][t][s] = ndp[i][j][t][s];
    }
    /* Sum over states where started == 1 and diff01 == diff04 == 0. */
    unsigned long long total = 0;
    for (int tight = 0; tight < 2; ++tight)
        total += dp[offset][offset][tight][1];
    return total;
}
 
/* Return the n-th integer whose base‑6 representation has equal numbers
 * of 0's, 1's and 4's.  n is one‑based.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for offset 0 */
    /* Determine an upper bound. */
    uint64_t low = 2;
    uint64_t high = 2;
    while (count_le(high) < (unsigned long long)n) {
        if (high > UINT64_MAX / 2) { high = UINT64_MAX; break; }
        high *= 2;
    }
    /* Binary search for smallest value with count >= n. */
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        if (count_le(mid) >= (unsigned long long)n)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
