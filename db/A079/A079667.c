/*
 * A079667  a(n) = (1/2) * Sum_{d divides n} abs(n/d-d).
 * https://oeis.org/A079667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/2) * Σ_{d|n} |n/d - d| (Integer square root and
 *            divisor enumeration)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor of the square root of a 64‑bit unsigned integer. */
static inline idx_t isqrt(idx_t n)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1ULL << 62;           /* largest power of four ≤ 2^64 */

    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Computes A079667. */
seq_t a(idx_t n)
{
    idx_t limit = isqrt(n);              /* largest divisor to consider */
    seq_t sum = 0;

    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;           /* complementary divisor */
            if (d < e)                 /* exclude the square root itself */
                sum += (seq_t)(e - d);
        }
    }
    return sum;
}
