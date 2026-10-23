/*
 * A156056  n-th triangular number modulo n-th prime.
 * https://oeis.org/A156056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*(n+1)/2) mod p_n, where p_n is the n-th prime
 * Range:     u8 n <= 22, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1, but return 0 for n==0. */
    if (n == 0)
        return (seq_t)0;

    seq_t tri = (seq_t)n * ((seq_t)n + (seq_t)1) / (seq_t)2;
    seq_t prime = (seq_t)get_prime(n);
    return tri % prime;
}
