/*
 * A135275  a(n) = prime(2n-1) + prime(2n) - prime(2n+1).
 * https://oeis.org/A135275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(2n-1) + prime(2n) - prime(2n+1)
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* External linkage as requested */
seq_t a(idx_t n)
{
    idx_t k1 = 2 * n - 1;
    idx_t k2 = 2 * n;
    idx_t k3 = 2 * n + 1;

    seq_t p1 = (seq_t)get_prime(k1);
    seq_t p2 = (seq_t)get_prime(k2);
    seq_t p3 = (seq_t)get_prime(k3);

    return p1 + p2 - p3;
}
