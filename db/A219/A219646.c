/*
 * A219646  Partial sums of A219642.
 * https://oeis.org/A219646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A219642(k); A219642(0)=0, A219642(k) = 1 +
 *            A219642(k - z(k)), z(k) = number of terms in Zeckendorf
 *            expansion of k, z(k) = 1 + z(k - F) with F the largest Fibonacci
 *            number <= k
 * Range:     u8 n <= 30, u16 n <= 654, u32 n <= 10946, u64 n <= 10946,
 *            u128 n <= 10946, big any n
 * Validated: standard, pass, 73 terms through n = 9341 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    size_t len = (size_t)n + 1;
    uint32_t *z = (uint32_t *)malloc(len * sizeof(uint32_t));
    uint32_t *s = (uint32_t *)malloc(len * sizeof(uint32_t));
    seq_t total = 0;
    uint64_t fa = 1, fb = 2; /* fa <= m < fb */
    size_t m;

    z[0] = 0;
    s[0] = 0;
    for (m = 1; m < len; m++) {
        while (fb <= (uint64_t)m) {
            uint64_t t = fa + fb;
            fa = fb;
            fb = t;
        }
        z[m] = z[m - (size_t)fa] + 1;
        s[m] = 1 + s[m - z[m]];
        total += (seq_t)s[m];
    }

    free(z);
    free(s);
    return total;
}
