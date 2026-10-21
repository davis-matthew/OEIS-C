/*
 * A090973  a(n) = ceiling(prime(n)/n).
 * https://oeis.org/A090973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(prime(n)/n)
 * Range:     u8 n <= 54, u16 n <= 6770, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* handle offset safely */
    seq_t p = (seq_t)get_prime(n);        /* nth prime, 1‑based */
    seq_t d = (seq_t)n;
    return (p + d - 1) / d;               /* ceiling division */
}
