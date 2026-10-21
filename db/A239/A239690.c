/*
 * A239690  Base 4 sum of digits of prime(n).
 * https://oeis.org/A239690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, hear
 * Formula:   a(n) = sum of base‑4 digits of the n‑th prime
 * Range:     u8 n <= 54, u16 n <= 6875, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t p = get_prime(n);
    seq_t sum = 0;
    while (p > 0) {
        sum += p & 3;
        p >>= 2;
    }
    return sum;
}
