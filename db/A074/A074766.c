/*
 * A074766  a(n) = prime(2n) - 2*prime(n) - n.
 * https://oeis.org/A074766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(2n) - 2*prime(n) - n
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the n-th and 2n-th primes.  get_prime returns the
       prime at the given 1-based index. */
    seq_t p_n  = get_prime(n);
    seq_t p_2n = get_prime(2ull * n);   /* 2*n in idx_t format */

    /* Compute the sequence value: prime(2n) - 2*prime(n) - n */
    seq_t result = p_2n - (seq_t)2 * p_n - (seq_t)n;
    return result;
}
