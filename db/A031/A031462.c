/*
 * A031462  Numbers whose base-4 representation has one more 0 than 3's.
 * https://oeis.org/A031462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth number whose base‑4 representation contains one more 0 than 3’s. */

#include <stdint.h>
#include <stdbool.h>

#define MAX_LEN 20
#define SHIFT   MAX_LEN
#define DP_SIZE (2 * MAX_LEN + 1)

static bool dp_inited = false;
static uint64_t dp[DP_SIZE][MAX_LEN + 1];   /* dp[diff+SHIFT][len] */

/* precompute dp table once */
static void init_dp(void)
{
    if (dp_inited) return;
    /* base case: length 0, success only if current diff == 1 */
    for (int d = 0; d < DP_SIZE; ++d) dp[d][0] = 0;
    dp[SHIFT + 1][0] = 1;

    /* DP: for each length l (1..MAX_LEN) */
    for (int l = 1; l <= MAX_LEN; ++l) {
        for (int dIdx = 0; dIdx < DP_SIZE; ++dIdx) {
            uint64_t sum = 0;
            /* digit 0: delta +1 */
            if (dIdx + 1 < DP_SIZE) sum += dp[dIdx + 1][l - 1];
            /* digit 3: delta -1 */
            if (dIdx > 0) sum += dp[dIdx - 1][l - 1];
            /* digits 1 and 2: delta 0 (two choices) */
            sum += dp[dIdx][l - 1];
            sum += dp[dIdx][l - 1];
            dp[dIdx][l] = sum;
        }
    }
    dp_inited = true;
}

/* nth term (1‑based index).  Returns 0 for n == 0 (undefined). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined */
    init_dp();

    uint64_t remaining = (uint64_t)n;   /* how many numbers still to skip */
    int L = 0;                          /* length of the base‑4 representation */

    /* Find needed length */
    for (int len = 1; ; ++len) {
        uint64_t count_len = 0;
        for (int first = 1; first <= 3; ++first) {   /* leading digit cannot be 0 */
            int delta = (first == 0) ? 1 : (first == 3) ? -1 : 0;
            int idx = SHIFT + delta;
            count_len += dp[idx][len - 1];
        }
        if (count_len < remaining) {
            remaining -= count_len;
        } else {
            L = len;
            break;
        }
    }

    /* Construct the value digit by digit */
    seq_t value = 0;
    int diff = 0;  /* current difference (#0 – #3) */

    for (int pos = 0; pos < L; ++pos) {
        int start = (pos == 0) ? 1 : 0;
        for (int digit = start; digit <= 3; ++digit) {
            int delta = (digit == 0) ? 1 : (digit == 3) ? -1 : 0;
            int newDiff = diff + delta;
            int rem = L - pos - 1;
            uint64_t cnt = dp[SHIFT + newDiff][rem];

            if (cnt < remaining) {
                remaining -= cnt;
            } else {
                value = value * (seq_t)4 + (seq_t)digit;
                diff = newDiff;
                break;
            }
        }
    }

    return value;
}
