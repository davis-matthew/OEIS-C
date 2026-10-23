/*
 * A025129  a(n) = p(1)p(n) + p(2)p(n-1) + ... + p(k)p(n-k+1), where k = [ n/2
 *          ], p = A000040, the primes.
 * https://oeis.org/A025129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊n/2⌋} p(i) * p(n-i+1), where p(k) is the k‑th
 *            prime
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 1017, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"  /* get_prime(idx_t k) returns the k‑th prime */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* outside offset */
    idx_t k = n / 2;                          /* floor(n/2) */
    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t p1 = (seq_t)get_prime(i);           /* p(i)   */
        seq_t p2 = (seq_t)get_prime(n - i + 1);    /* p(n-i+1) */
        sum += p1 * p2;
    }
    return sum;
}
