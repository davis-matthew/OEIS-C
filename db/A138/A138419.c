/*
 * A138419  a(n) = (prime(n)^4 - prime(n)^2)/3.
 * https://oeis.org/A138419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)^4 - prime(n)^2)/3
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 66, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* nth prime, 1-indexed */
    uint32_t p32 = get_prime(n);
    seq_t p   = (seq_t)p32;
    seq_t p2  = p * p;
    seq_t p4  = p2 * p2;
    seq_t res = (p4 - p2) / (seq_t)3;
    return res;
}
