/*
 * A110496  Least k such that prime(n)^3 divides binomial(2k,k).
 * https://oeis.org/A110496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)^3 + 1)/2 for n > 1, a(1) = 7
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   cube, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/cube.h"

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)7;
    }
    seq_t p = get_prime((uint64_t)n);
    seq_t c = cube(p);
    return (c + (seq_t)1) / (seq_t)2;
}
