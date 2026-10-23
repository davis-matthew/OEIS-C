/*
 * A138032  a(n) = prime(n)^17.
 * https://oeis.org/A138032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^17
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 6, u128 n <= 42,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Grab the n-th prime (1‑based). */
    seq_t prime = (seq_t)get_prime(n);
    /* Raise it to the 17th power. */
    return big_pow(prime, (seq_t)17);
}
