/*
 * A172022  a(n) = prime(n) + (-1)^n.
 * https://oeis.org/A172022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) + (-1)^n
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime. get_prime returns a uint32_t, cast to seq_t. */
    seq_t p = (seq_t)get_prime(n);

    /* Add 1 for even n, subtract 1 for odd n. */
    if ((n & 1) == 0) {
        return p + 1;
    } else {
        return p - 1;
    }
}
