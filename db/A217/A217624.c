/*
 * A217624  Prime(prime(3*n)).
 * https://oeis.org/A217624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_{p_{3n}} (nth prime of the (3n)th prime)
 * Range:     u8 n <= 5, u16 n <= 281, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* index of the three‑times argument */
    idx_t k = 3 * n;
    /* p_{3n} */
    idx_t p3 = (idx_t)get_prime(k);
    /* p_{p_{3n}} */
    seq_t result = (seq_t)get_prime(p3);
    return result;
}
