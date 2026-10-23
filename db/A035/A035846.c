/*
 * A035846  Coordination sequence for A_20 lattice.
 * https://oeis.org/A035846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d=1}^{20} C(21,d) * C(n-1,d-1) * C(20-d+n, n) with
 *            a(0) = 1 (n in non‑negative integers)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: binomial with small k, k <= 20 (or 19), normalise to smaller k. */
static seq_t binom_small(idx_t N, idx_t k) {
    if (k > N) return 0;
    if (k > N - k) k = N - k;          /* use smaller side, always <= 10 */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply then divide, guaranteed divisible */
        res = (res * (N - k + i)) / i;
    }
    return res;
}

/* Pre‑computed binomial(21,d) for d = 0..20; computed lazily */
static seq_t C21[21];
static bool C21_init = false;

static void init_C21(void) {
    if (C21_init) return;
    C21_init = true;
    for (idx_t d = 0; d <= 20; ++d) {
        /* compute C(21,d) with small k */
        C21[d] = binom_small(21, d);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    init_C21();

    seq_t sum = 0;
    for (idx_t d = 1; d <= 20; ++d) {
        /* binom(n-1, d-1) is zero if d > n */
        if (d > n) break;

        seq_t b1 = binom_small(n - 1, d - 1);          /* C(n-1, d-1) */
        idx_t a_val = n + 20 - d;                     /* n + (20-d) */
        idx_t k_small = 20 - d;                       /* a_val - n */
        seq_t b2 = binom_small(a_val, k_small);       /* C(20-d+n, n) */
        seq_t term = C21[d] * b1 * b2;
        sum += term;
    }
    return sum;
}
