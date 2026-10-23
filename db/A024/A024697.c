/*
 * A024697  a(n) = p(1)p(n) + p(2)p(n-1) + ... + p(k)p(n+1-k), where k = [
 *          (n+1)/2 ], p = A000040 = the primes.
 * https://oeis.org/A024697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{floor((n+1)/2)} p(i)*p(n+1-i) where p(k) is
 *            the k‑th prime number
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 1017, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The offset for this sequence is 1, so a(1) is the first term.
       For n < 1 the behavior is irrelevant, but we guard against it. */
    if (n == 0) return 0;

    idx_t k = (n + 1) / 2;            /* floor((n+1)/2) */
    seq_t sum = 0;

    for (idx_t i = 1; i <= k; ++i) {
        seq_t pi = (seq_t)get_prime(i);
        seq_t pj = (seq_t)get_prime(n + 1 - i);
        sum += pi * pj;
    }
    return sum;
}
