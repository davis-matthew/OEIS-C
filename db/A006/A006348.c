/*
 * A006348  a(n) = (n+2)*a(n-1) + (-1)^n.
 * https://oeis.org/A006348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*a(n-1) + (-1)^n, a(1) = 0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;

    if (n >= memo_len) {
        idx_t new_len = n + 256;
        seq_t *new_memo = alloc_zero(new_len);
        if (memo) {
            for (idx_t i = 0; i < memo_len; i++) new_memo[i] = memo[i];
            release(memo, memo_len);
        }
        memo = new_memo;
        memo_len = new_len;
    }

    if (computed_up_to < 1) {
        memo[1] = 0;
        computed_up_to = 1;
    }

    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t v = memo[i - 1] * (seq_t)(i + 2);
        if (i % 2 == 0) v += 1;
        else v -= 1;
        memo[i] = v;
        computed_up_to = i;
    }

    return memo[n];
}
