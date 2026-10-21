/*
 * A070848  Smallest prime == 1 mod (4n).
 * https://oeis.org/A070848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 732, u32 n <= 6097, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) = smallest prime p such that p ≡ 1 (mod 4n) */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* compute the modulus 4*n as a seq_t value */
    seq_t mod = (seq_t)4 * n;
    /* first candidate in the arithmetic progression 1 mod 4n */
    seq_t p = mod + (seq_t)1;

    /* iterate until a prime is found */
    while (!is_prime_big(p))
        p += mod;

    return p;
}
