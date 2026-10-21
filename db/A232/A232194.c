/*
 * A232194  Number of ways to write n = x + y (x, y > 0) with n*x + y and n*y
 *          - x both prime.
 * https://oeis.org/A232194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs x > 0, y > 0 with x + y = n such that n*x
 *            + y and n*y - x are both prime
 * Range:     u8 n <= 5642, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 1; x < n; ++x) {
        idx_t y  = n - x;
        idx_t p1 = n * x + y;
        idx_t p2 = n * y - x;
        if (is_prime(p1) && is_prime(p2))
            ++count;
    }
    return count;
}
