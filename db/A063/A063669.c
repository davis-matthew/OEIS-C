/*
 * A063669  Hypotenuses of reciprocal Pythagorean triangles: number of
 *          solutions to 1/(12n)^2 = 1/b^2 + 1/c^2 [with b >= c > 0]; also
 *          number of values of A020885 (with repetitions) which divide n.
 * https://oeis.org/A063669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primitive Pythagorean triples (m,k), m>k>=1,
 *            gcd(m,k)=1, m-k odd, such that A020885-value m*k*(m^2-k^2)/6
 *            divides n (search over Euclid parameters; with k=1 giving the
 *            smallest value for each m)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A063669_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t count = 0;
    for (uint64_t m = 2; ; ++m) {
        /* smallest value for this m occurs at k = 1 */
        if (m * (m * m - 1) / 6 > N) break;
        for (uint64_t k = 1; k < m; ++k) {
            if (((m - k) & 1) == 0) continue;
            /* value is not monotonic in k, so no early break here */
            uint64_t v = m * k * (m - k) * (m + k) / 6;
            if (v > N) continue;
            if (N % v != 0) continue;
            if (A063669_gcd(m, k) != 1) continue;
            ++count;
        }
    }
    return (seq_t)count;
}
