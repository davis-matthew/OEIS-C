/*
 * A138444  a(n) = ((n-th prime)^6-(n-th prime^2))/5.
 * https://oeis.org/A138444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((prime(n))^6 - (prime(n))^2)/5
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* Get the nth prime */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* Compute p^2, p^4, p^6 */
    seq_t p2 = p * p;
    seq_t p4 = p2 * p2;
    seq_t p6 = p4 * p2;

    /* (p^6 - p^2) / 5 */
    seq_t num = p6 - p2;
    return num / (seq_t)5;
}
