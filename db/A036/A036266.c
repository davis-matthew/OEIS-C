/*
 * A036266  5th differences of primes.
 * https://oeis.org/A036266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = p(n+5) - 5*p(n+4) + 10*p(n+3) - 10*p(n+2) + 5*p(n+1) -
 *            p(n)
 * Range:     i8 n <= 214, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p(k) is the k-th prime (p(1)=2) */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* fetch primes at positions n .. n+5 */
    seq_t p0 = (seq_t)get_prime(n);
    seq_t p1 = (seq_t)get_prime(n + 1);
    seq_t p2 = (seq_t)get_prime(n + 2);
    seq_t p3 = (seq_t)get_prime(n + 3);
    seq_t p4 = (seq_t)get_prime(n + 4);
    seq_t p5 = (seq_t)get_prime(n + 5);

    /* compute the 5th finite difference */
    seq_t res = p5
                - 5 * p4
                + 10 * p3
                - 10 * p2
                + 5 * p1
                - p0;
    return res;
}
