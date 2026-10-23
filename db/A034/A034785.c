/*
 * A034785  a(n) = 2^(n-th prime).
 * https://oeis.org/A034785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^A000040(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* offset 1 - a(1) is first term */
    uint32_t p = get_prime(n);    /* p = nth prime */
    return pow2((idx_t)p);        /* 2^p */
}
