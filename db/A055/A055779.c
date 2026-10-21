/*
 * A055779  Number of fat trees on n labeled vertices.
 * https://oeis.org/A055779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k)*k^(n-k)*n^(k-2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"      /* provides static seq_t binom(idx_t, idx_t) */

/* Helper: exponentiation by squaring over seq_t */
static seq_t seq_pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1u) result *= base;
        exp >>= 1u;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term;
        if (k == 1) {
            /* binomial(n,1)*1^(n-1)*n^(-1) = 1 for all n >= 1 */
            term = 1;
        } else {
            term = binom(n, k);                    /* C(n,k)          */
            term *= seq_pow_seq_t((seq_t)k, n - k); /* k^(n-k)         */
            term *= seq_pow_seq_t((seq_t)n, k - 2); /* n^(k-2)         */
        }
        total += term;
    }
    return total;
}
