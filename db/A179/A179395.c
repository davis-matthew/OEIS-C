/*
 * A179395  a(n) = prime(n)^2 mod prime(n+3).
 * https://oeis.org/A179395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^2 mod prime(n+3)
 * Range:     u8 n <= 51, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    seq_t p1 = (seq_t) get_prime(n);
    seq_t p2 = (seq_t) get_prime(n + 3);
    return (p1 * p1) % p2;
}
