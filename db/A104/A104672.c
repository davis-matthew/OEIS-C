/*
 * A104672  a(n) = binomial(n+4,n)*binomial(n+9,n).
 * https://oeis.org/A104672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+4,n) * binomial(n+9,n) = binomial(n+4,4) *
 *            binomial(n+9,9)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 14, u64 n <= 99, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute C(N,K) for small K (here K ≤ 9). */
static seq_t binom_small(idx_t N, idx_t K) {
    if (K > N - K) K = N - K;          /* take smaller argument */
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        /* multiply then divide – exact division each step */
        res = (seq_t)((unsigned __int128)res * (N - K + i) / i);
    }
    return res;
}

seq_t a(idx_t n) {
    seq_t b1 = binom_small(n + 4, 4);   /* C(n+4,4) */
    seq_t b2 = binom_small(n + 9, 9);   /* C(n+9,9) */
    return (seq_t)((unsigned __int128)b1 * b2);
}
