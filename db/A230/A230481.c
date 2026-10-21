/*
 * A230481  a(n) = prime(prime(2*n)) - prime(2*prime(n)).
 * https://oeis.org/A230481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(prime(2*n)) - prime(2*prime(n))
 * Range:     i8 n <= 13, i16 n <= 1624, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1U)
        return (seq_t)0;

    /* 2*n is always >= 2, so safe */
    idx_t two_n = 2 * n;
    idx_t p2n   = (idx_t)get_prime(two_n);            /* prime(2*n) */

    /* prime(p2n) */
    idx_t primed_p2n = (idx_t)get_prime(p2n);

    /* prime(n) */
    idx_t pn = (idx_t)get_prime(n);

    /* prime(2*pn) */
    idx_t two_pn = 2 * pn;
    idx_t prime_2pn = (idx_t)get_prime(two_pn);

    /* compute signed difference */
    long long diff = (long long)primed_p2n - (long long)prime_2pn;
    return (seq_t)diff;
}
