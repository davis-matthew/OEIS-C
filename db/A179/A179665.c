/*
 * A179665  a(n) = prime(n)^9.
 * https://oeis.org/A179665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^9
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 33, u128 n <= 1000,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* retrieve the n-th prime (prime(1) = 2) */
    seq_t p = (seq_t)get_prime(n);

    /* compute p^9 by repeated multiplication */
    seq_t result = p;
    for (int i = 1; i < 9; ++i)
        result *= p;

    return result;
}
