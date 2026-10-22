/*
 * A000477  a(n) = Sum_{k=1..n-1} k^2*sigma(k)*sigma(n-k).
 * https://oeis.org/A000477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2/24)*(3*sigma_3(n) - (4n-1)*sigma_1(n)), i.e. (n^2/24
 *            - n^3/6)*sigma_1(n) + (n^2/8)*sigma_3(n)
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 125, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000477_sigmas
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000477_sigmas.h"

seq_t a(idx_t n) {
    seq_t s1, s3, t;
    uint64_t n2, g, x, y;
    a000477_sigmas(n, &s1, &s3);
    /* t = 24*a(n)/n^2, which is no larger than a(n) for n >= 5 */
    t = s3 * 3 - s1 * (seq_t)(4 * n - 1);
    n2 = (uint64_t)n * (uint64_t)n;
    x = n2;
    y = 24;
    while (y) {
        uint64_t r = x % y;
        x = y;
        y = r;
    }
    g = x;
    return (seq_t)(n2 / g) * (t / (seq_t)(24 / g));
}
