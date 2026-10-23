/*
 * A031373  a(n) = prime(7n-1).
 * https://oeis.org/A031373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(7*n-1)
 * Range:     u8 n <= 7, u16 n <= 934, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    idx_t idx = 7 * n - 1;
    return (seq_t)get_prime(idx);
}
