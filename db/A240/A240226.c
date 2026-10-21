/*
 * A240226  4-adic value of 1/n, n >= 1.
 * https://oeis.org/A240226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 if n is odd; otherwise a(n) = 4^ceil(v2(n)/2), where
 *            v2(n) is the exponent of 2 in n. This is equivalent to a(n) =
 *            4^valuation(2*n,4). The sequence is multiplicative with a(2^e) =
 *            4^ceil(e/2) and a(p^e) = 1 for odd primes p
 * Range:     u8 n <= 127, u16 n <= 9825, u32 n <= 9825, u64 n <= 9825,
 *            u128 n <= 9825, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Odd indices */
    if ((n & 1) != 0)
        return (seq_t)1;

    /* Compute v2(n) */
    idx_t k = 0;
    idx_t tmp = n;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        k += 1;
    }

    /* f = ceil(k/2) */
    idx_t f = (k + 1) / 2;

    /* 4^f in seq_t */
    seq_t result = 1;
    for (idx_t i = 0; i < f; ++i)
        result *= 4;

    return result;
}
