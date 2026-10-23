/*
 * A036263  Second differences of primes.
 * https://oeis.org/A036263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = prime(n) + prime(n+2) - 2*prime(n+1)
 * Range:     i8 n <= 9922, i16 n <= 9922, i32 n <= 9922, i64 n <= 9922,
 *            i128 n <= 9922, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Convert indices to seq_t to avoid accidental overflows on very
     * constrained seq_t types. */
    seq_t p_n   = (seq_t)get_prime(n);
    seq_t p_n1  = (seq_t)get_prime(n + 1);
    seq_t p_n2  = (seq_t)get_prime(n + 2);

    /* Compute: prime(n) + prime(n+2) - 2*prime(n+1) */
    return p_n + p_n2 - (p_n1 + p_n1);
}
