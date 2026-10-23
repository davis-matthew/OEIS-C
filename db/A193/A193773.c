/*
 * A193773  Number of ways to write n as 2*x*y - x - y with 1 <= x <= y.
 * https://oeis.org/A193773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( d(2*n+1) / 2 ) where d(k) is the number of positive
 *            divisors of k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute the number of divisors of a 64‑bit integer */
static seq_t _divisor_count_u64(uint64_t x)
{
    seq_t d = 1;
    for (uint64_t p = 2; p <= x / p; ++p) {
        if (x % p == 0) {
            seq_t e = 0;
            while (x % p == 0) {
                x /= p;
                e += 1;
            }
            d *= (e + 1);
        }
    }
    if (x > 1)                      /* remaining prime factor */
        d *= 2;
    return d;
}

seq_t a(idx_t n)
{
    /* m = 2*n + 1   (fits in 64 bits for the target range of n) */
    uint64_t m = (uint64_t)2 * (uint64_t)n + 1;
    seq_t d = _divisor_count_u64(m);
    /* ceil(d/2)  ==  (d+1)/2 for integer arithmetic */
    return (d + 1) / 2;
}
