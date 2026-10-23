/*
 * A031166  a(n) = prime(n+4) - prime(n).
 * https://oeis.org/A031166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+4) - prime(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The function assumes n ≥ 1 as per the OEIS offset. */
    return (seq_t)get_prime(n + 4) - (seq_t)get_prime(n);
}
