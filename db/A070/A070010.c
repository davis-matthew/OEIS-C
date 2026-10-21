/*
 * A070010  GCD of consecutive values of sum-of-proper divisors.
 * https://oeis.org/A070010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( sum_of_proper_divisors(n) ,
 *            sum_of_proper_divisors(n+1) )
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute the sum of all divisors of n (including n) using prime factorisation */
static seq_t sigma_of(idx_t n) {
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t pp = 1;
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
                pp *= (seq_t)p;              /* pp = p^exp */
            }
            /* sigma(p^exp) = (p^(exp+1)-1)/(p-1) */
            seq_t numerator   = pp * (seq_t)p - 1; /* p^(exp+1) - 1 */
            seq_t denominator = (seq_t)p - 1;      /* p - 1 */
            term = numerator / denominator;
            result *= term;
        }
    }
    if (m > 1) {                 /* remaining prime factor */
        seq_t term = ((seq_t)m * (seq_t)m - 1) / ((seq_t)m - 1); /* p + 1 */
        result *= term;
    }
    return result;
}

/* Sum of proper divisors = sum_of_divisors - n */
static seq_t proper_sum(idx_t n) {
    seq_t sigma = sigma_of(n);
    return sigma - (seq_t)n;     /* sigma >= n for n ≥ 1 */
}

/* Required sequence function */
seq_t a(idx_t n) {
    seq_t s_n   = proper_sum(n);
    seq_t s_np1 = proper_sum(n + 1);
    return gcd(s_n, s_np1);
}
