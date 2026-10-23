/*
 * A131946  Expansion of (phi(-q) * phi(-q^3))^2 in powers of q where phi() is
 *          a Ramanujan theta function.
 * https://oeis.org/A131946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -4 * b(n) , where * b is multiplicative with * b(2^e) = 3
 *            - 2^(e+1) * b(3^e) = 1 * b(p^e) = (p^(e+1) - 1) / (p - 1) for p
 *            > 3 * a(0) = 1
 * Range:     i8 n <= 12, i16 n <= 2791, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer power for seq_t: base^exp; exp is a small unsigned integer */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    idx_t m = n;
    seq_t res = (seq_t)1;

    while (m > 1) {
        /* find the smallest prime divisor of m */
        idx_t p = 0;
        for (idx_t i = 2; i * i <= m; ++i) {
            if (m % i == 0) {
                p = i;
                break;
            }
        }
        if (p == 0)          /* m itself is prime */
            p = m;

        /* count exponent of p in m */
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        seq_t factor;
        if (p == 2) {
            seq_t pow2 = pow_seq_t((seq_t)2, e + 1);   /* 2^(e+1) */
            factor = (seq_t)3 - pow2;                  /* 3 - 2^(e+1) */
        } else if (p == 3) {
            factor = (seq_t)1;                         /* b(3^e) = 1 */
        } else {
            seq_t powp = pow_seq_t((seq_t)p, e + 1);   /* p^(e+1) */
            factor = (powp - (seq_t)1) / ((seq_t)p - (seq_t)1);
        }

        res *= factor;
    }

    return (seq_t)-4 * res;
}
