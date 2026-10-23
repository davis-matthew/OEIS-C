/*
 * A116570  a(2*n) = prime(n+1) * prime(n+2), a(2*n-1) = prime(n+1).
 * https://oeis.org/A116570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(2*n) = prime(n+1) * prime(n+2), a(2*n-1) = prime(n+1)
 * Range:     u8 n <= 11, u16 n <= 107, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if ((n & 1U) == 0U) {
        idx_t m = n >> 1;
        seq_t p1 = (seq_t)get_prime(m + 1);
        seq_t p2 = (seq_t)get_prime(m + 2);
        return p1 * p2;
    } else {
        idx_t m = (n + 1) >> 1;
        return (seq_t)get_prime(m + 1);
    }
}
