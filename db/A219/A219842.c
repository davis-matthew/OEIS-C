/*
 * A219842  Number of ways to write n as x+y (0<x<=y) with 2x*y+1 prime.
 * https://oeis.org/A219842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs 0 < x <= y, x+y=n, such that 2*x*y+1 is
 *            prime
 * Range:     u8 n <= 22, u16 n <= 2591, u32 n <= 2591, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */
    seq_t count = 0;
    idx_t x;
    for (x = 1; x <= n / 2; ++x) {
        idx_t y = n - x;
        seq_t p = (seq_t)2 * (seq_t)x * (seq_t)y + 1;
        if (is_prime_big(p)) ++count;
    }
    return count;
}
