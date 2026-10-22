/*
 * A007663  Fermat quotients: (2^(p-1)-1)/p, where p=prime(n).
 * https://oeis.org/A007663
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (2^(p-1)-1)/p, p = prime(n); computed by exact long
 *            division of the all-ones binary number 2^(p-1)-1 by p, 16 bits
 *            at a time
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 32,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"

static uint64_t nth_prime(idx_t n) {
    if (n == 1) return 2;
    idx_t count = 1;
    uint64_t candidate = 3;
    while (1) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        candidate += 2;
    }
}

seq_t a(idx_t n) {
    uint64_t p = nth_prime(n);
    uint64_t bits = p - 1;          /* 2^(p-1)-1 has p-1 one-bits */
    uint64_t r = 0;
    seq_t q = 0;
    unsigned k = (unsigned)(bits % 16);
    if (k == 0) k = 16;
    while (bits > 0) {
        uint64_t cur = (r << k) + ((1ULL << k) - 1);
        uint64_t d = cur / p;
        r = cur % p;
        seq_t mul = (seq_t)(1ULL << k);
        q = q * mul + (seq_t)d;
        bits -= k;
        k = 16;
    }
    return q;
}
