/*
 * A004797  Convolution of A002024 with itself.
 * https://oeis.org/A004797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} A002024(i+1)*A002024(n+1-i), where
 *            A002024(k) = m for m(m-1)/2 < k <= m(m+1)/2
 * Range:     u8 n <= 16, u16 n <= 287, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    idx_t len = n + 2;
    uint64_t *b = (uint64_t *)malloc(len * sizeof(uint64_t));
    uint64_t m = 1, tri = 1;
    idx_t k, i;
    seq_t s = 0;
    b[0] = 0;
    for (k = 1; k < len; k++) {
        while (tri < k) {
            m++;
            tri += m;
        }
        b[k] = m;
    }
    for (i = 0; i <= n; i++) {
        s += (seq_t)b[i + 1] * (seq_t)b[n + 1 - i];
    }
    free(b);
    return s;
}
