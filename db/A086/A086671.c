/*
 * A086671  Sum of floor(sqrt(d)) where d runs through the divisors of n.
 * https://oeis.org/A086671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} floor(sqrt(d))
 * Range:     u8 n <= 1259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* integer square root: largest integer x with x*x <= n */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x = 0;
    uint64_t bit = 1ULL << 62;             /* highest power of 4 ≤ 2^64-1 */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for n = 0, though offset is 1 */
    uint64_t limit = isqrt_u64((uint64_t)n);
    seq_t sum = 0;

    for (uint64_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            sum += (seq_t)isqrt_u64(d);
            uint64_t e = n / d;
            if (e != d) {
                sum += (seq_t)isqrt_u64(e);
            }
        }
    }
    return sum;
}
