/*
 * A161769  A positive integer k is included if the greatest common divisor of
 *          (the sum of the distinct primes dividing k) and (the sum of the
 *          exponents in the prime factorization of k) is > 1.
 * https://oeis.org/A161769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer k such that gcd( sum of
 *            distinct prime divisors of k, sum of exponents in the prime
 *            factorization of k ) > 1
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used, but harmless */
    uint64_t count = 0;
    uint64_t k = 1;
    while (count < n) {
        ++k;
        uint64_t primes[16];
        uint64_t exps[16];
        size_t pc = 0;
        factor_u64(k, primes, exps, &pc);
        uint64_t sumPr  = 0;
        uint64_t sumExp = 0;
        for (size_t i = 0; i < pc; ++i) {
            sumPr  += primes[i];
            sumExp += exps[i];
        }
        if (gcd((seq_t)sumPr, (seq_t)sumExp) > (seq_t)1) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
    return 0;   /* Unreachable */
}
