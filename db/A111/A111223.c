/*
 * A111223  Numbers n such that 5*n + 2 is prime.
 * https://oeis.org/A111223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer m such that 5*m + 2 is prime
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    uint64_t cur = 0;
    idx_t    cnt = 0;

    while (cnt < n) {
        uint64_t val = 5ull * cur + 2;
        if (is_prime(val)) {
            ++cnt;
            if (cnt == n)
                return (seq_t)cur;
        }
        ++cur;
    }
    return 0; /* unreachable, keeps the compiler happy */
}
