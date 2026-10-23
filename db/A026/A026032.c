/*
 * A026032  a(n) = C(3n,n) - C(3n,n-4).
 * https://oeis.org/A026032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(3n,n) - C(3n,n-4)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Multiplicative computation of binomial coefficient C(N,K) */
static seq_t binom_calc(idx_t N, idx_t K) {
    if (K > N) return 0;
    if (K > N - K) K = N - K;   /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= K; ++i) {
        result = result * (seq_t)(N - K + i) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t binom1 = binom_calc(3 * n, n);
    seq_t binom2 = 0;
    if (n >= 4) binom2 = binom_calc(3 * n, n - 4);
    return binom1 - binom2;
}
