/*
 * A227378  Smallest number with n = sum of distinct digits in decimal
 *          representation, cf. A217928.
 * https://oeis.org/A227378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest integer whose set of distinct decimal digits sums to n.
   For 0 <= n <= 45 the minimal number is built using digits 1..9 only.
   For n > 45 no such number exists; a(n) is defined as 0. */
#include <stdint.h>
#include <stdbool.h>

static seq_t dp[46];
static bool init_done = false;

static void init_dp(void) {
    static bool valid[46] = {0};
    valid[0] = true;
    dp[0] = 0;
    for (int d = 1; d <= 9; ++d) {
        for (int s = 45; s >= 0; --s) {
            if (!valid[s]) continue;
            int ns = s + d;
            if (ns > 45) continue;
            seq_t new_val = dp[s] * 10 + (seq_t)d;
            if (!valid[ns] || new_val < dp[ns]) {
                dp[ns] = new_val;
                valid[ns] = true;
            }
        }
    }
    init_done = true;
}

seq_t a(idx_t n) {
    if (!init_done) init_dp();
    if (n <= 45) {
        return dp[(int)n];
    } else {
        return 0;  /* Undefined for n > 45 */
    }
}
