/*
 * A208548  a(n) = floor((4/3)*(2+prime(n)) - prime(n+1)).
 * https://oeis.org/A208548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4/3)*(2 + prime(n)) - prime(n+1))
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        /* Sequence offset is 1; return 0 for undefined indices */
        return 0;
    }

    /* Fetch the n-th and (n+1)-th primes */
    seq_t pn  = (seq_t)get_prime(n);
    seq_t pn1 = (seq_t)get_prime(n + 1);

    /* Compute floor(4*(pn+2)/3) and subtract pn1 */
    seq_t temp = (seq_t)4 * (pn + 2) / 3;
    return temp - pn1;
}
