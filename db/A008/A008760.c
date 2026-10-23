/*
 * A008760  Expansion of (1+x^17)/((1-x)*(1-x^2)*(1-x^3)).
 * https://oeis.org/A008760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(n) + (n>=17 ? f(n-17) : 0) where f(k) = (k*k + 6*k +
 *            12) / 12 (The generating function (1+x^17)/((1-x)(1-x^2)(1-x^3))
 *            expands to a(n) = floor((n^2+6n+12)/12) +
 *            floor(((n-17)^2+6(n-17)+12)/12) for n>=0.)
 * Range:     u8 n <= 43, u16 n <= 632, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for exit() */

static inline seq_t f(idx_t k)
{
    seq_t kk = (seq_t)k;
    return (kk * kk + (seq_t)6 * kk + (seq_t)12) / (seq_t)12;
}

seq_t a(idx_t n)
{
    seq_t result = f(n);
    if (n >= 17)
        result += f(n - 17);
    return result;
}
