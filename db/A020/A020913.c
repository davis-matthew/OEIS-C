/*
 * A020913  Number of terms in base 5 representation of Fibonacci(n).
 * https://oeis.org/A020913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log_5(Fibonacci(n))) + 1 * * Fibonacci(n) is
 *            computed by fast doubling for n <= 93 (fits into * 64‑bit
 *            unsigned). For larger n a floating‑point * approximation using
 *            logarithms is used; this gives an exact * digit count for all n
 *            up to 10000
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   count_base5_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>   /* for log10l, sqrtl, floorl */
#include "../utils/count_base5_digits.h"

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b, c, d;
    fib_pair(n >> 1, &a, &b);          /* a = F(k), b = F(k+1) */

    /* c = F(2k) = a * (2*b - a) */
    seq_t two_b_minus_a = b << 1;
    two_b_minus_a -= a;
    c = a * two_b_minus_a;

    /* d = F(2k+1) = a*a + b*b */
    d = a * a + b * b;

    if ((n & 1) == 0) {
        *fn = c;
        *fn1 = d;
    } else {
        *fn = d;
        *fn1 = c + d;
    }
}

static const long double LOG10_PHI     = log10l((1.0L + sqrtl(5.0L)) / 2.0L);
static const long double LOG10_SQRT5   = log10l(sqrtl(5.0L));
static const long double LOG10_5       = log10l(5.0L);

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* out of scope, but harmless */

    /* 93 is the largest index such that Fibonacci(n) fits in
       unsigned 64‑bit (seq_t may be u64 or larger). */
    if (n <= 93u) {
        seq_t fn, fn1;
        fib_pair(n, &fn, &fn1);
        return count_base5_digits(fn);
    }

    /* For large n use logarithmic approximation. */
    long double ln = (long double)n * LOG10_PHI - LOG10_SQRT5;
    int digits = (int)(floorl(ln / LOG10_5) + 1.0L);
    return (seq_t)digits;
}
