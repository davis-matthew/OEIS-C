/*
 * A078153  a(n) = A051201(n) - A000203(n).
 * https://oeis.org/A078153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (sum of distinct values of floor(n/j), j=1..n) - sigma(n)
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma(n) is the sum of all divisors of n. */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

/* integer square root: floor(sqrt(x)) */
static idx_t isqrt_idx(idx_t x) {
    /* use long double to avoid overflow for very large x; adjust if needed */
    long double d = sqrt((long double)x);
    idx_t r = (idx_t)d;
    while ((idx_t)(r + 1) * (idx_t)(r + 1) <= x) ++r;
    while ((idx_t)r * (idx_t)r > x) --r;
    return r;
}

seq_t a(idx_t n) {
    /* sum of distinct floor(n/j) values */
    idx_t t = isqrt_idx(n);
    seq_t sum_dist = 0;
    for (idx_t k = 1; k <= t; ++k) {
        seq_t val = (seq_t)n / (seq_t)k;   /* floor division */
        if (val == (seq_t)k)
            sum_dist += val;               /* avoid double count */
        else
            sum_dist += val + (seq_t)k;
    }

    /* sum of divisors sigma(n) via prime factorization */
    seq_t sigma = 1;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            seq_t p_pow = 1;
            seq_t term_sum = 1;
            while (m % p == 0) {
                m /= p;
                p_pow *= (seq_t)p;
                term_sum += p_pow;
            }
            sigma *= term_sum;
        }
    }
    if (m > 1) { /* remaining prime factor */
        sigma *= ((seq_t)1 + (seq_t)m);
    }

    return sum_dist - sigma;
}
