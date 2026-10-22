/*
 * A005427  Josephus problem: numbers m such that, when m people are arranged
 *          on a circle and numbered 1 through m, the final survivor when we
 *          remove every 4th person is one of the first three people.
 * https://oeis.org/A005427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 + ceiling(Sum_{k=1..n-1} a(k)/3), a(1) = 5. The running
 *            sum S is kept as q = floor(S/3) and r = S mod 3, so no
 *            intermediate exceeds about a(n)
 * Range:     u8 n <= 14, u16 n <= 33, u32 n <= 72, u64 n <= 149,
 *            u128 n <= 303, big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t computed_up_to = 0;
static seq_t sum_q = 0;
static int sum_r = 0;

seq_t a(idx_t n) {
    if (memo_len < n) {
        idx_t new_len = memo_len == 0 ? 16 : memo_len;
        while (new_len < n) new_len *= 2;
        memo = grow_zero(memo, memo_len, new_len);
        memo_len = new_len;
    }

    while (computed_up_to < n) {
        seq_t val = sum_q + 5;
        if (sum_r > 0) val += 1;
        memo[computed_up_to] = val;
        computed_up_to++;
        sum_q += val / 3;
        sum_r += (int)(long long)(val % 3);
        if (sum_r >= 3) {
            sum_r -= 3;
            sum_q += 1;
        }
    }

    return memo[n - 1];
}
