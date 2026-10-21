/*
 * A239885  a(n) = 2^(n-2) * prime(n).
 * https://oeis.org/A239885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2) * prime(n)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 27, u64 n <= 57, u128 n <= 120,
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
    /* OEIS offset 1: a(1) = 1. Handle it specially. */
    if (n == 0) return 0;               /* safety for n < 1 */
    if (n == 1) return (seq_t)1;

    /* prime(n) fits in uint32_t for expected n (≤ 10⁴). */
    seq_t p = (seq_t)get_prime(n);

    /* Compute 2^(n-2) using arbitrary-precision multiplication. */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n - 2));

    return p * pow2;
}
