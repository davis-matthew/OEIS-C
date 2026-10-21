/*
 * A069539  Duplicate of A063997.
 * https://oeis.org/A069539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 14, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A069539
 * Numbers divisible by 4 whose decimal digit sum is 4, listed in increasing order.
 * This implementation uses combinatorial counting with dynamic programming
 * to compute the nth term in O(len*10) time, where len is the number of digits
 * of a(n).  The pre‑computed table has size 201 × 5 × 4 and fits in a few kilobytes.
 */

#include <stdint.h>
#include <stdbool.h>

/* Number of positions to pre‑compute.
 * 200 guarantees that the 10 000th term of this sequence is covered. */
#define MAX_LEN 200

/* Dynamic programming table:
 * ways[pos][sum][mod] = number of ways to fill exactly 'pos' digits
 * (from most significant to least) such that, given the current
 * remainder of the prefix modulo 4 equals 'mod', the final remainder is 0,
 * the total digit sum of the suffix is 'sum', and digits are allowed to be 0‑9.
 */
static uint64_t ways[MAX_LEN + 1][5][4];

/* Counts the number of length‑len terms (no leading zero) satisfying the
 * conditions.  The first digit is forced to be 1‑9. */
static uint64_t count_len(idx_t len)
{
    uint64_t total = 0;
    for (int d = 1; d <= 9 && d <= 4; ++d) {
        total += ways[(int)len - 1][4 - d][d % 4];
    }
    return total;
}

/* Fill the DP table once.  This function is called lazily by a(). */
static void init_dp(void)
{
    /* Base case: zero remaining positions. */
    for (int mod = 0; mod < 4; ++mod) {
        ways[0][0][mod] = (mod == 0) ? 1 : 0;
    }

    /* All other sum>0 are impossible when no positions remain. */
    for (int sum = 1; sum <= 4; ++sum) {
        for (int mod = 0; mod < 4; ++mod) {
            ways[0][sum][mod] = 0;
        }
    }

    /* Fill for positions 1..MAX_LEN. */
    for (int pos = 1; pos <= MAX_LEN; ++pos) {
        for (int sum = 0; sum <= 4; ++sum) {
            for (int mod = 0; mod < 4; ++mod) {
                uint64_t total = 0;
                for (int d = 0; d <= 9 && d <= sum; ++d) {
                    int new_mod = (mod * 10 + d) & 3;       /* mod 4 using bit‑wise AND */
                    total += ways[pos - 1][sum - d][new_mod];
                }
                ways[pos][sum][mod] = total;
            }
        }
    }
}

/* Return the nth term of the sequence (0‑indexed). */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_dp();
        initialized = true;
    }

    /* Find the length of a(n). */
    idx_t idx = n;
    idx_t len = 1;
    while (true) {
        uint64_t cnt = count_len(len);
        if (idx < cnt) {
            break;
        }
        idx -= cnt;
        ++len;
    }

    /* Construct the number digit by digit. */
    seq_t ans = 0;
    int mod = 0;    /* current remainder of the built prefix */
    int sum = 4;    /* remaining digit sum to be used */

    for (idx_t pos = 0; pos < len; ++pos) {
        int start_d = (pos == 0) ? 1 : 0;       /* leading digit cannot be 0 */
        for (int d = start_d; d <= 9 && d <= sum; ++d) {
            int new_mod = (mod * 10 + d) & 3;
            int new_sum = sum - d;
            int rem = (int)len - (int)pos - 1;  /* remaining positions after this digit */
            uint64_t cnt = ways[rem][new_sum][new_mod];
            if (cnt <= idx) {
                idx -= cnt;
                continue;
            }
            /* choose this digit */
            ans = ans * 10 + d;
            mod = new_mod;
            sum = new_sum;
            break;
        }
    }
    return ans;
}
