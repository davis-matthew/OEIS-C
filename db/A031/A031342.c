/*
 * A031342  a(n) = prime(9*n).
 * https://oeis.org/A031342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(9*n)
 * Range:     u8 n <= 6, u16 n <= 726, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; the sequence is undefined for n < 1 */
    idx_t k = n * 9u;
    /* get_prime returns uint32_t, cast it to seq_t */
    return (seq_t)get_prime(k);
}
