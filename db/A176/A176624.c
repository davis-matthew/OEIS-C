/*
 * A176624  a(n) = prime(n) + n*(-1)^n.
 * https://oeis.org/A176624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + n*(-1)^n
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term; n must be ≥ 1. */
    if (n < 1)
        return (seq_t)0;               /* undefined behaviour for n < 1 */

    seq_t p = (seq_t)get_prime(n);       /* nth prime (1‑indexed) */

    if (n & 1ULL)                       /* n is odd: subtract n  */
        return p - (seq_t)n;
    else                                /* n is even: add n   */
        return p + (seq_t)n;
}
