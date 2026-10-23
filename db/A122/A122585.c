/*
 * A122585  Reciprocal of n modulo smallest prime greater than n.
 * https://oeis.org/A122585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the multiplicative inverse of n modulo the smallest prime
 *            > n
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Find the smallest prime greater than n. */
    idx_t p = n + 1;
    while (!is_prime(p))
        ++p;

    /* Compute the inverse of n modulo p using Fermat’s little theorem. */
    return mod_pow((seq_t)n, (uint64_t)(p - 2), (seq_t)p);
}
