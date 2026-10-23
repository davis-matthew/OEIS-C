/*
 * A132227  a(n) = 3*prime(n) - 5.
 * https://oeis.org/A132227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 3*prime(n) - 5
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    seq_t p = (seq_t)get_prime(n);
    return 3 * p - 5;
}
