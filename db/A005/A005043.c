/*
 * A005043  Riordan numbers: a(n) = (n-1)*(2*a(n-1) + 3*a(n-2))/(n+1).
 * https://oeis.org/A005043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = (n-1)*(2*a(n-1) + 3*a(n-2))/(n+1), a(0) = 1, a(1) = 0;
 *            evaluated as (p/g) * ((2*a(n-1) + 3*a(n-2)) / ((n+1)/g)) with p
 *            = n-1, g = gcd(n-1, n+1), splitting the inner division into
 *            quotients and remainders to avoid overflow
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 25, u64 n <= 46, u128 n <= 87,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 0;

    if (memo_len == 0) {
        memo = grow_zero(memo, 0, 2);
        memo[0] = 1;
        memo[1] = 0;
        memo_len = 2;
    }
    if (n >= memo_len) {
        memo = grow_zero(memo, memo_len, n + 1);
        for (idx_t i = memo_len; i <= n; i++) {
            idx_t g = (i & 1) ? 2 : 1;
            seq_t p = (seq_t)((i - 1) / g);
            seq_t m = (seq_t)((i + 1) / g);
            seq_t q1 = memo[i - 1] / m, r1 = memo[i - 1] % m;
            seq_t q2 = memo[i - 2] / m, r2 = memo[i - 2] % m;
            seq_t q = 2 * q1 + 3 * q2 + (2 * r1 + 3 * r2) / m;
            memo[i] = p * q;
        }
        memo_len = n + 1;
    }
    return memo[n];
}
