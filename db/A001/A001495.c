/*
 * A001495  Number of symmetric 0-1 (n+1) X (n+1) matrices with row sums 2 and
 *          first row starting 1,1 for n > 0, a(0)=1.
 * https://oeis.org/A001495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*a(n-1) + (n-2)^2*a(n-2) - (n-2)(n-3)(n-4)*a(n-3) -
 *            (1/2)(n-2)(n-3)(n-4)*a(n-4) - (1/2)(n-2)(n-3)(n-4)(n-5)*a(n-5),
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=3, a(4)=13
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)13;

    /* rolling window: v0 = a(i-5), ..., v4 = a(i-1) */
    seq_t v0 = (seq_t)1, v1 = (seq_t)1, v2 = (seq_t)1, v3 = (seq_t)3, v4 = (seq_t)13;
    idx_t i;
    for (i = 5; i <= n; i++) {
        uint64_t c1 = (uint64_t)(i - 1);
        uint64_t c2 = (uint64_t)(i - 2) * (uint64_t)(i - 2);
        uint64_t p3 = (uint64_t)(i - 2) * (uint64_t)(i - 3) * (uint64_t)(i - 4);
        uint64_t c4 = p3 / 2;                       /* product of 3 consecutive is even */
        uint64_t c5 = (p3 / 2) * (uint64_t)(i - 5);

        seq_t pos = (seq_t)c1 * v4;
        pos += (seq_t)c2 * v3;
        seq_t neg = (seq_t)p3 * v2;
        neg += (seq_t)c4 * v1;
        neg += (seq_t)c5 * v0;
        seq_t next = pos - neg;

        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = next;
    }
    return v4;
}
