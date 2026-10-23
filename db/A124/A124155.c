/*
 * A124155  19^p - 18^p, where p = Prime[n].
 * https://oeis.org/A124155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19^Prime[n] - 18^Prime[n]
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Obtain the n‑th prime (1‑based). */
    seq_t prime = (seq_t)get_prime(n);

    /* Compute 19^prime and 18^prime using arbitrary precision power. */
    seq_t pow19 = big_pow((seq_t)19, prime);
    seq_t pow18 = big_pow((seq_t)18, prime);

    return pow19 - pow18;
}
