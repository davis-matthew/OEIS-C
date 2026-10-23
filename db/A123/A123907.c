/*
 * A123907  a(n) = T(p(n)) - p(T(n)) = Commutator[triangular numbers, primes]
 *          at n.
 * https://oeis.org/A123907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, less
 * Formula:   a(n) = T(p(n)) - p(T(n)) where T(i) = i*(i+1)/2 and p(i) = i-th
 *            prime
 * Range:     i8 n <= 9, i16 n <= 66, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) /* undefined for n < 1, return 0 */
        return (seq_t)0;

    /* get the n-th prime */
    uint32_t prime_n = get_prime(n);
    /* T(p(n)) = p(n)*(p(n)+1)/2 */
    seq_t triangular_prime = (seq_t)prime_n * ((seq_t)prime_n + 1) / 2;

    /* compute T(n) as idx_t since it is used as an index */
    idx_t triangular_n = n * (n + 1) / 2;
    /* get the prime at position T(n) */
    uint32_t prime_tri_n = get_prime(triangular_n);
    seq_t triangular_prime_of_tri_n = (seq_t)prime_tri_n;

    return triangular_prime - triangular_prime_of_tri_n;
}
