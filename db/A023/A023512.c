/*
 * A023512  Exponent of 2 in prime factorization of prime(n) + 1.
 * https://oeis.org/A023512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = exponent of 2 dividing prime(n)+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exp_of_two, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/exp_of_two.h"

/* OEIS sequence A023512:
   a(n) = v₂(prime(n)+1)   (offset 1). */
seq_t a(idx_t n)
{
    /* get the n-th prime (offset 1) */
    uint32_t p = get_prime(n);

    /* p+1 as seq_t */
    seq_t v = (seq_t)p + 1;

    return exp_of_two(v);
}
