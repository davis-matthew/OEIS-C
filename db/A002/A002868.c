/*
 * A002868  Largest number in n-th row of triangle of Lah numbers (A008297 and
 *          A271703).
 * https://oeis.org/A002868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = max_k L(n,k), where L(n,k) = binomial(n-1,k-1)*n!/k! are
 *            the Lah numbers. L(n,k+1)/L(n,k) = (n-k)/(k*(k+1)), so the row
 *            is unimodal and the maximum is at the smallest k >= 1 with n-k
 *            <= k*(k+1). a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t k, i;
    seq_t c, r;
    if (n == 0) return 1;
    k = 1;
    while (n - k > k * (k + 1)) k++;
    /* c = binomial(n-1, k-1), computed multiplicatively */
    c = 1;
    for (i = 1; i < k; i++) {
        c = c * (seq_t)(n - k + i);
        c = c / (seq_t)i;
    }
    /* r = c * (k+1)*(k+2)*...*n */
    r = c;
    for (i = k + 1; i <= n; i++) r = r * (seq_t)i;
    return r;
}
