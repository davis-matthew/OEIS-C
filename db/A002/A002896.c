/*
 * A002896  Number of 2n-step polygons on cubic lattice.
 * https://oeis.org/A002896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk, nice
 * Formula:   a(n) = C(2*n, n)*Sum_{k=0..n} C(n, k)^2*C(2*k, k), summand built
 *            incrementally: t(k+1) = t(k)*(n-k)^2*2*(2k+1)/(k+1)^3 with exact
 *            steps
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t t = (seq_t)1;     /* C(n,k)^2 * C(2k,k) */
    seq_t sum = (seq_t)1;   /* k = 0 term */
    seq_t cb = (seq_t)1;    /* C(2n,n) */
    idx_t k;
    for (k = 0; k < n; k++) {
        /* C(n,k)^2 C(2k,k) -> C(n,k) C(n,k+1) C(2k,k) */
        t = t * (seq_t)(n - k);
        t = t / (seq_t)(k + 1);
        /* -> C(n,k+1)^2 C(2k,k) */
        t = t * (seq_t)(n - k);
        t = t / (seq_t)(k + 1);
        /* -> C(n,k+1)^2 C(2k+2,k+1) */
        t = t * (seq_t)(2 * (2 * k + 1));
        t = t / (seq_t)(k + 1);
        sum += t;

        cb = cb * (seq_t)(2 * (2 * k + 1));
        cb = cb / (seq_t)(k + 1);
    }
    return cb * sum;
}
