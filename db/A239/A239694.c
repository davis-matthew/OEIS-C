/*
 * A239694  Base 8 sum of digits of prime(n).
 * https://oeis.org/A239694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A053829(A000040(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    uint32_t p = get_prime(n);
    seq_t sum = 0;
    while (p) {
        sum += p % 8;
        p /= 8;
    }
    return sum;
}
