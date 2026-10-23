/*
 * A181831  The sum of positive integers <= n that are strongly prime to n.
 * https://oeis.org/A181831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * phi(n)/2 - sigma(n-1) * where phi is Euler's totient
 *            and sigma is the sum of divisors. * For n = 0 or 1, a(n) = 0.
 *            This implements A181831
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t phi_seq(idx_t n)
{
    seq_t result = (seq_t)n;
    idx_t tmp = n;

    if (tmp % 2 == 0) {
        result -= result / 2;
        while (tmp % 2 == 0) tmp /= 2;
    }
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            result -= result / (seq_t)p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) result -= result / (seq_t)tmp;
    return result;
}

static seq_t sigma_seq(idx_t n)
{
    seq_t result = 1;
    idx_t tmp = n;

    /* factor 2 separately */
    if (tmp % 2 == 0) {
        idx_t e = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            ++e;
        }
        /* term = (2^(e+1)-1)/(2-1) = 2^(e+1)-1 */
        seq_t pow = 1;
        for (idx_t i = 0; i < e + 1; ++i) pow *= (seq_t)2;
        seq_t term = pow - 1;  /* divisor = 1 */
        result *= term;
    }

    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            /* term = (p^(e+1)-1)/(p-1) */
            seq_t pow = 1;
            for (idx_t i = 0; i < e + 1; ++i) pow *= (seq_t)p;
            seq_t numerator = pow - 1;
            seq_t denominator = (seq_t)(p - 1);
            seq_t term = numerator / denominator;
            result *= term;
        }
    }

    if (tmp > 1) {
        /* remaining prime factor with exponent 1 */
        seq_t term = (seq_t)(tmp + 1);  /* (p^(2)-1)/(p-1) = p+1 */
        result *= term;
    }

    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* sum of totatives: n * phi(n) / 2 */
    seq_t phi = phi_seq(n);
    seq_t sum_tot = (seq_t)n * phi / 2;

    /* subtract sum of divisors of n-1 */
    seq_t sigma = sigma_seq(n - 1);

    return sum_tot - sigma;
}
