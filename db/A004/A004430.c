/*
 * A004430  Geometric mean of digits of n (rounded up).
 * https://oeis.org/A004430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceiling((product of the decimal digits of n)^(1/k)),
 *            where k is the number of digits of n; a(0) = 0
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t prod = 1;
    unsigned int k = 0;
    idx_t t = n;
    unsigned int r, i;

    if (n == 0) return (seq_t)0;

    while (t > 0) {
        unsigned int d = (unsigned int)(t % 10);
        if (d == 0) return (seq_t)0;
        prod *= d;          /* at most 9^20 < 2^64 */
        k++;
        t /= 10;
    }

    /* smallest r in 1..9 with r^k >= prod (the geometric mean never exceeds 9) */
    for (r = 1; r < 9; r++) {
        uint64_t p = 1;
        for (i = 0; i < k; i++) p *= r;   /* r^k <= 8^20 < 2^64 */
        if (p >= prod) break;
    }
    return (seq_t)r;
}
