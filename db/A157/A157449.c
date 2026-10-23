/*
 * A157449  Difference between n and the sum of its divisors except 1 and
 *          itself.
 * https://oeis.org/A157449
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n+1) - sigma(n) where sigma(n) is the sum of all
 *            positive divisors of n
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static seq_t sum_of_divisors(uint64_t n) {
    if (n == 0ULL) return 0;           // unused but safe guard
    if (n == 1ULL) return (seq_t)1;    // sigma(1) = 1
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);       // obtain prime factors and exponents
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        int e = (int)ex[i];
        seq_t term = 1;                // sum of powers of p: 1 + p + ... + p^e
        seq_t power = 1;
        for (int k = 1; k <= e; ++k) {
            power *= p;
            term += power;
        }
        sigma *= term;
    }
    return sigma;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              // undefined for n<2, return 0
    seq_t n_seq = (seq_t)n;
    seq_t sigma = sum_of_divisors((uint64_t)n);
    return (2 * n_seq + 1) - sigma;
}
