/*
 * A230482  a(n) = (prime(prime(2*n)) - prime(2*prime(n)))/2.
 * https://oeis.org/A230482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (prime(prime(2*n)) - prime(2*prime(n))) / 2
 * Range:     i8 n <= 13, i16 n <= 1976, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1, but not needed */

    /* indices for the needed primes */
    idx_t idx_2n   = n << 1;                    /* 2*n */
    uint32_t p_n   = get_prime(n);             /* prime(n) */
    uint32_t p_2n  = get_prime(idx_2n);        /* prime(2*n) */

    uint32_t p_pp2n = get_prime((idx_t)p_2n);  /* prime(prime(2*n)) */
    uint32_t p_2pn  = get_prime((idx_t)(p_n << 1)); /* prime(2*prime(n)) */

    seq_t diff = (seq_t)p_pp2n - (seq_t)p_2pn;
    return diff / 2;
}
