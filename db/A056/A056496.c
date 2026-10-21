/*
 * A056496  Number of primitive (period n) periodic palindromes using a
 *          maximum of five different symbols.
 * https://oeis.org/A056496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * Sum_{i=0..2} binom(5,i) * 5^{(n/d -
 *            i)/2} (term added only if (n/d - i) is even and >=0)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* Binomial coefficients C(5,i) for i = 0..2 */
static const seq_t _binom5[3] = {(seq_t)1, (seq_t)5, (seq_t)10};

/* Compute the Möbius value of n (n >= 1) */
static int mobius(idx_t n)
{
    if (n == 1) return 1;
    idx_t t = n;
    int cnt = 0;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int exp = 0;
            do {
                t /= p;
                ++exp;
                if (exp > 1) return 0;          /* squared prime factor */
            } while (t % p == 0);
            ++cnt;
        }
    }
    if (t > 1) ++cnt;                          /* remaining prime factor */
    return (cnt & 1) ? -1 : 1;
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* outside offset – no value */

    seq_t result = 0;

    /* Enumerate all divisors of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        /* First divisor d */
        {
            int mu = mobius(d);
            if (mu != 0) {
                idx_t t = n / d;
                seq_t contrib = 0;
                for (int i = 0; i <= 2; ++i) {
                    if (t < (idx_t)i) continue;
                    idx_t diff = t - (idx_t)i;
                    if (diff & 1u) continue;   /* (t - i) must be even */
                    idx_t exp = diff >> 1;      /* (t - i)/2   */
                    seq_t pow5 = big_pow((seq_t)5, (seq_t)exp);
                    contrib += pow5 * _binom5[i];
                }
                if (mu > 0) result += contrib;
                else        result -= contrib; /* mu == -1  */
            }
        }

        /* Second divisor d2 = n/d, if distinct */
        idx_t d2 = n / d;
        if (d2 != d) {
            int mu = mobius(d2);
            if (mu != 0) {
                idx_t t = n / d2;
                seq_t contrib = 0;
                for (int i = 0; i <= 2; ++i) {
                    if (t < (idx_t)i) continue;
                    idx_t diff = t - (idx_t)i;
                    if (diff & 1u) continue;
                    idx_t exp = diff >> 1;
                    seq_t pow5 = big_pow((seq_t)5, (seq_t)exp);
                    contrib += pow5 * _binom5[i];
                }
                if (mu > 0) result += contrib;
                else        result -= contrib;
            }
        }
    }
    return result;
}
