/*
 * A052617  E.g.f. (1+x-x^2)/((1-x)(1-2x)).
 * https://oeis.org/A052617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 (n = 0) a(n) = (5*2^(n-1) - 1) * n! for n > 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t fact = 1;
    seq_t pow2 = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;     /* factorial n! */
        if (i < n)
            pow2 *= 2;       /* precompute 2^(n-1) */
    }

    seq_t term = (seq_t)5 * pow2; /* 5*2^(n-1) */
    term -= 1;
    term *= fact;                /* multiply by n! */
    return term;
}
