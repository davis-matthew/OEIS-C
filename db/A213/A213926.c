/*
 * A213926  prime(n^2) - prime(n).
 * https://oeis.org/A213926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n^2) - prime(n)
 * Range:     u8 n <= 7, u16 n <= 81, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t pn = (seq_t)get_prime(n);
    seq_t pn2 = (seq_t)get_prime(n * n);
    return pn2 - pn;
}
