/*
 * A127348  Coefficient of x^2 in the polynomial
 *          (x-p(n))*(x-p(n+1))*(x-p(n+2))*(x-p(n+3)), where p(k) is the k-th
 *          prime.
 * https://oeis.org/A127348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n)*p(n+1) + p(n)*p(n+2) + p(n)*p(n+3) * + p(n+1)*p(n+2)
 *            + p(n+1)*p(n+3) + p(n+2)*p(n+3)
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* p(k) is the k-th prime (1‑based). */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* retrieve the four successive primes */
    seq_t p0 = (seq_t)get_prime(n);
    seq_t p1 = (seq_t)get_prime(n + 1);
    seq_t p2 = (seq_t)get_prime(n + 2);
    seq_t p3 = (seq_t)get_prime(n + 3);

    /* sum of all pairwise products */
    seq_t res = p0 * p1 + p0 * p2 + p0 * p3
              + p1 * p2 + p1 * p3 + p2 * p3;

    return res;
}
