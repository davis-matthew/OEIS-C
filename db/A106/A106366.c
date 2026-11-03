/*
 * A106366  Number of necklaces with n beads of 4 colors, no 2 adjacent beads
 *          the same color.
 * https://oeis.org/A106366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 for n==1; a(n) = (1/n) * Σ_{d|n} φ(n/d) * (3*(-1)^d +
 *            3^d) for n>1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 43, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 171 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

/* fast exponentiation of 3^exp, exp >= 0 */
static seq_t pow3(idx_t exp)
{
    seq_t result = 1;
    seq_t base = 3;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Euler's totient φ(m),  m >= 1 */
static seq_t phi_of(idx_t m)
{
    idx_t x = m;
    seq_t res = m;
    for (idx_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            while (x % p == 0)
                x /= p;
            res -= res / p;
        }
    }
    if (x > 1)
        res -= res / x;
    return res;
}

seq_t a_wide(idx_t n)
{
    if (n == 1)
        return (seq_t)4;

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        idx_t d1 = d;
        idx_t d2 = n / d;

        /* first divisor d1 */
        seq_t phi1 = phi_of(d2);
        seq_t pow1 = pow3(d1);
        seq_t coeff1 = pow1;
        if (d1 & 1) {
            /* d1 is odd: coeff = 3^d1 - 3 */
            coeff1 -= (seq_t)3;
        } else {
            /* d1 is even: coeff = 3^d1 + 3 */
            coeff1 += (seq_t)3;
        }
        sum += phi1 * coeff1;

        /* second divisor d2 if distinct */
        if (d2 != d1) {
            seq_t phi2 = phi_of(d1);
            seq_t pow2 = pow3(d2);
            seq_t coeff2 = pow2;
            if (d2 & 1) {
                coeff2 -= (seq_t)3;
            } else {
                coeff2 += (seq_t)3;
            }
            sum += phi2 * coeff2;
        }
    }

    return sum / n;
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
