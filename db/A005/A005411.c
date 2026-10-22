/*
 * A005411  Number of non-vanishing Feynman diagrams of order 2n for the
 *          electron or the photon propagators in quantum electrodynamics.
 * https://oeis.org/A005411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = u(n+1) for n > 0, a(0) = 1, where u(1) = 1 and u(n) =
 *            (2*n - 4)*u(n-1) + Sum_{k=1..n-1} u(k)*u(n-k) for n > 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t memo_computed = 0;

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }

    idx_t target = n + 1;

    if (memo == NULL) {
        memo_len = (target < 1024) ? 1024 : target + 100;
        memo = alloc_zero(memo_len);
        memo[1] = 1;
        memo_computed = 1;
    }

    if (target >= memo_len) {
        idx_t new_len = target + target / 2 + 100;
        memo = grow_zero(memo, memo_len, new_len);
        memo_len = new_len;
    }

    for (idx_t i = memo_computed + 1; i <= target; i++) {
        seq_t sum = 0;
        idx_t half = i / 2;
        for (idx_t k = 1; k < i - k; k++) {
            sum += memo[k] * memo[i - k];
        }
        sum *= 2;
        if (i % 2 == 0) {
            sum += memo[half] * memo[half];
        }
        seq_t coeff = (seq_t)(i - 2) * 2;
        memo[i] = coeff * memo[i - 1] + sum;
    }

    if (target > memo_computed) {
        memo_computed = target;
    }

    return memo[target];
}
