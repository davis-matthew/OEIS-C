/*
 * A037916  Concatenate exponents in prime factorization of n.
 * https://oeis.org/A037916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of exponents in the prime factorization of
 *            n, with exponents listed in ascending order of the corresponding
 *            primes
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

static int digits_uint64(uint64_t x) {
    int d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    uint64_t val = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(val, primes, exps, &cnt);
    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t exp = exps[i];
        int d = digits_uint64(exp);
        seq_t pow10 = 1;
        for (int j = 0; j < d; ++j) pow10 *= 10;
        res = res * pow10 + (seq_t)exp;
    }
    return res;
}
