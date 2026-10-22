/*
 * A007557  Shifts left when inverse Moebius transform applied twice.
 * https://oeis.org/A007557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(n+1) = Sum_{d|n} tau(n/d)*a(d), a(1) = 1
 * Range:     u8 n <= 16, u16 n <= 69, u32 n <= 498, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"

static seq_t *memo = NULL;
static idx_t memo_cap = 0;   /* allocated length of memo */
static idx_t memo_len = 0;   /* highest index computed */
static idx_t *tau_table = NULL;
static idx_t tau_len = 0;

static void compute_tau_table(idx_t up_to) {
    if (tau_len >= up_to) return;
    idx_t new_tau_len = tau_len == 0 ? 16 : tau_len * 2;
    while (new_tau_len < up_to) new_tau_len *= 2;
    idx_t *new_tau = (idx_t *)malloc((size_t)(new_tau_len + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= new_tau_len; i++) new_tau[i] = 0;
    for (idx_t i = 1; i <= new_tau_len; i++)
        for (idx_t j = i; j <= new_tau_len; j += i)
            new_tau[j]++;
    if (tau_table) free(tau_table);
    tau_table = new_tau;
    tau_len = new_tau_len;
}

static void ensure_computed(idx_t up_to) {
    if (up_to <= memo_len) return;
    compute_tau_table(up_to);

    if (up_to + 1 > memo_cap) {
        idx_t new_cap = memo_cap == 0 ? 32 : memo_cap * 2;
        while (new_cap < up_to + 1) new_cap *= 2;
        memo = grow_zero(memo, memo_cap, new_cap);
        memo_cap = new_cap;
    }

    if (memo_len == 0) {
        memo[1] = 1;
        memo_len = 1;
    }

    for (idx_t k = memo_len + 1; k <= up_to; k++) {
        idx_t m = k - 1;
        seq_t result = 0;
        for (idx_t d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t c = m / d;
                result += (seq_t)tau_table[c] * memo[d];
                if (d != c) result += (seq_t)tau_table[d] * memo[c];
            }
        }
        memo[k] = result;
    }
    memo_len = up_to;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n == 1) return 1;
    ensure_computed(n);
    return memo[n];
}
