/*
 * A003314  Binary entropy function: a(1)=0; for n > 1, a(n) = n + min {
 *          a(k)+a(n-k) : 1 <= k <= n-1 }.
 * https://oeis.org/A003314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*floor(log_2(4n)) - 2^floor(log_2(2n)), i.e. with k =
 *            floor(log_2(n)): a(n) = n*(k+2) - 2^(k+1)
 * Range:     u8 n <= 45, u16 n <= 5266, u32 n <= 9255, u64 n <= 9255,
 *            u128 n <= 9255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t k = 0;
    idx_t m = n;
    while (m > 1) {
        m >>= 1;
        k++;
    }
    seq_t p = (seq_t)1;
    for (idx_t i = 0; i <= k; i++) {
        p = p + p;
    }
    seq_t r = (seq_t)n * (seq_t)(k + 2);
    r -= p;
    return r;
}
