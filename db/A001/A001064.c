/*
 * A001064  a(n) = a(n-1)*a(n-2) + a(n-3).
 * https://oeis.org/A001064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*a(n-2) + a(n-3), a(0) = 1, a(1) = 1, a(2) = 0
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 13, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t memo[20001];
static idx_t memo_len = 3;
static int initialized = 0;

seq_t a(idx_t n) {
    if (!initialized) {
        memo[0] = 1;
        memo[1] = 1;
        memo[2] = 0;
        initialized = 1;
    }
    
    if (n >= 20001) {
        seq_t prev3 = 1, prev2 = 1, prev1 = 0;
        for (idx_t i = 3; i <= n; i++) {
            seq_t curr = prev1 * prev2 + prev3;
            prev3 = prev2;
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
    
    if (n >= memo_len) {
        for (idx_t i = memo_len; i <= n; i++) {
            memo[i] = memo[i-1] * memo[i-2] + memo[i-3];
        }
        memo_len = n + 1;
    }
    
    return memo[n];
}
