/*
 * A234741  a(n) is the base-2 carryless product of the prime factors of n;
 *          Encoding of the product of the polynomials over GF(2) represented
 *          by the prime factors of n (with multiplicity).
 * https://oeis.org/A234741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = carryless product (over GF(2)) of the prime factors of n
 *            (including multiplicity). For n>=1, a(1)=1. Implemented by
 *            factoring n via trial division and performing polynomial
 *            multiplication modulo 2 on the primes
 * Range:     u8 n <= 255, u16 n <= 9925, u32 n <= 9925, u64 n <= 9925,
 *            u128 n <= 9925, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Carryless multiplication of two unsigned integers (GF(2)[x]) */
static inline seq_t carryless_mul (seq_t x, seq_t y)
{
    seq_t r = 0;
    while (y) {
        if (y & 1) r ^= x;
        x <<= 1;
        y >>= 1;
    }
    return r;
}

/* Sequence A234741 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out-of-range */
    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p * p <= m; ++p) {
        while (m % p == 0) {
            result = carryless_mul(result, (seq_t)p);
            m /= p;
        }
    }
    if (m > 1) { /* remaining prime factor greater than sqrt(n) */
        result = carryless_mul(result, (seq_t)m);
    }
    return result;
}
