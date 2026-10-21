/*
 * A074284  Sum of the aliquot divisors of n-th triangular number.
 * https://oeis.org/A074284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ(n*(n+1)/2) – n*(n+1)/2 (sum of proper divisors of the
 *            n-th triangular number)
 * Range:     u8 n <= 19, u16 n <= 223, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute the n-th triangular number as a 64‑bit integer (sufficient for n ≤ 10 000). */
    uint64_t n64 = (uint64_t)n;
    uint64_t tri = n64 * (n64 + 1ULL) / 2ULL;

    /* Factorise tri and compute σ(tri) using the multiplicative property of the divisor‑sum function */
    uint64_t temp = tri;
    uint64_t sigma = 1ULL;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t power   = 1;      /* p^0 initially */
            uint64_t sum_pow = 1;      /* 1+p+...+p^e */

            /* Accumulate the power of p and the sum 1 + p + ... + p^e   */
            while (temp % p == 0) {
                temp /= p;
                power *= p;
                sum_pow += power;
            }
            sigma *= sum_pow;
        }
    }
    if (temp > 1) {
        /* Remaining prime factor */
        sigma *= (1 + temp);
    }

    /* Sum of proper divisors = σ(tri) – tri */
    seq_t result = (seq_t)sigma - (seq_t)tri;
    return result;
}
