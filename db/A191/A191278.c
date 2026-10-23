/*
 * A191278  Count of mosaic numbers that equal n.
 * https://oeis.org/A191278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (∏_j binomial(beta, e_j)) / beta, where beta =
 *            bigOmega(n)+1
 * Range:     u8 n <= 71, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) for small n,k using integer arithmetic */
static inline seq_t _my_binom(idx_t n, idx_t k) {
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        /* res = res * (n - k + i) / i;  all divisions are exact */
        res = (res * (n - k + i)) / i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n <= (idx_t)1) return (seq_t)1;   /* a(1) = 1 */

    uint64_t m = (uint64_t)n;
    idx_t exponents[64];
    idx_t cnt = 0;
    idx_t total_exp = 0;

    /* Trial division factorisation */
    for (uint64_t p = (uint64_t)2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            exponents[cnt++] = e;
            total_exp += e;
        }
    }
    if (m > 1) {   /* Remaining prime factor */
        exponents[cnt++] = 1;
        total_exp += 1;
    }

    idx_t beta = total_exp + 1;          /* beta = bigOmega(n) + 1 */
    seq_t prod = (seq_t)1;

    for (idx_t i = 0; i < cnt; ++i) {
        prod = prod * _my_binom(beta, exponents[i]);
    }
    prod = prod / (seq_t)beta;           /* divide by beta (exact) */
    return prod;
}
