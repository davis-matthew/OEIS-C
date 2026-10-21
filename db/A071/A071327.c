/*
 * A071327  Sum of the squared primes dividing n.
 * https://oeis.org/A071327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p^2|n} p^2 (sum of squares of primes that divide n
 *            with exponent at least 2)
 * Range:     u8 n <= 288, u16 n <= 9816, u32 n <= 9816, u64 n <= 9816,
 *            u128 n <= 9816, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    uint64_t t = (uint64_t)n;

    /* handle factor 2 */
    uint64_t cnt = 0;
    while ((t & 1ULL) == 0) { /* t % 2 == 0 */
        ++cnt;
        t >>= 1;               /* t /= 2; */
    }
    if (cnt >= 2) {
        seq_t sq = (seq_t)2 * (seq_t)2;
        result += sq;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        cnt = 0;
        while (t % p == 0) {
            ++cnt;
            t /= p;
        }
        if (cnt >= 2) {
            seq_t sq = (seq_t)p * (seq_t)p;
            result += sq;
        }
    }

    /* if remainder t > 1 it is a prime with exponent 1, so nothing to add */

    return result;
}
