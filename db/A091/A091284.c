/*
 * A091284  Exponent of 2 in the prime factorization of prime(n)^8 - 1.
 * https://oeis.org/A091284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = v2(prime(n)-1) + v2(prime(n)+1) + 2 (for n ≥ 2), a(1) =
 *            0, where v2(x) is the exponent of 2 in the factorisation of x
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

static seq_t v2(seq_t x)
{
    /* Count the trailing zeros in the binary representation of x. */
    seq_t cnt = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 1)          /* prime(1) = 2 → 2⁸–1 = 255 is odd */
        return 0;

    /* nth prime (returns a 32‑bit value; cast to seq_t). */
    seq_t p = (seq_t)get_prime(n);

    /* p is odd for all n ≥ 2, so p–1 and p+1 are even. */
    seq_t v1 = v2(p - 1);
    seq_t v2_ = v2(p + 1);

    /* v2(8) = 3; the LTE formula gives v2(p⁸–1) = v2(p–1)+v2(p+1)+v2(8)−1. */
    return v1 + v2_ + 2;            /* 2 = v2(8)−1 */
}
