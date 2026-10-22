/*
 * A005318  Conway-Guy sequence: a(n + 1) = 2a(n) - a(n - floor( 1/2 +
 *          sqrt(2n) )).
 * https://oeis.org/A005318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n+1) = 2*a(n) - a(n - floor(1/2 + sqrt(2n))), a(0) = 0, a(1) =
 *            1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   a000037_isqrt, alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/a000037_isqrt.h"
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

/* floor(1/2 + sqrt(2m)) computed exactly with integers */
static uint64_t round_sqrt_2m(idx_t m) {
    uint64_t two_m = 2 * (uint64_t)m;
    uint64_t k = a000037_isqrt(two_m);
    if (two_m > k * k + k) {
        k++;
    }
    return k;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    if (n >= memo_len) {
        idx_t old_len = memo_len;
        idx_t start;
        if (memo == NULL) {
            memo = alloc_zero(n + 1);
            memo[0] = 0;
            memo[1] = 1;
            start = 2;
        } else {
            memo = grow_zero(memo, memo_len, n + 1);
            start = old_len;
        }
        memo_len = n + 1;

        for (idx_t i = start; i <= n; i++) {
            idx_t m = i - 1;
            idx_t back_idx = m - (idx_t)round_sqrt_2m(m);
            memo[i] = 2 * memo[m] - memo[back_idx];
        }
    }

    return memo[n];
}
