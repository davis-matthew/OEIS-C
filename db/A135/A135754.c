/*
 * A135754  E.g.f.: A(x) = Sum_{n>=0} exp((4^n-1)/3*x)*x^n/n!.
 * https://oeis.org/A135754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binom(n,k) * ((4^k-1)/3)^(n-k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* multiplicative binomial coefficient C(n,k) */
static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / ((seq_t)i);
    }
    return res;
}

/* fast exponentiation: base^exp (exp is idx_t) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1)
            result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow4 = 1; /* 4^k, starts with k = 0 */
    for (idx_t k = 0; k <= n; ++k) {
        /* binomial coefficient for this k */
        seq_t bin = binom_calc(n, k);
        /* base = (4^k - 1) / 3 */
        seq_t base = (pow4 - 1) / 3;
        /* power = base^(n-k) */
        seq_t power = pow_seq(base, n - k);
        sum += bin * power;
        /* update 4^k for next iteration */
        pow4 = pow4 * 4;
    }
    return sum;
}
