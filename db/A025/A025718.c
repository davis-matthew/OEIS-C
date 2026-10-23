/*
 * A025718  Index of 6^n within the sequence of the numbers of the form
 *          6^i*9^j.
 * https://oeis.org/A025718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=0..n} ceil(k * log_9(6))
 * Range:     u8 n <= 23, u16 n <= 399, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static const long double LOG9_6 = logl(6.0L) / logl(9.0L);

seq_t a(idx_t n)
{
    /* a(0) = 1, the term k = 0 contributes nothing to the sum */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;                      /* Σ ceil(k * LOG9_6) for k=1..n */
    for (idx_t k = 1; k <= n; ++k) {
        long double prod = (long double)k * LOG9_6;
        /* Since LOG9_6 is irrational, prod is never an integer.
           floor(prod) is obtained by truncating the long double to
           a long long.  ceil(prod) = floor(prod) + 1.             */
        seq_t term = (seq_t)((long long)prod + 1);
        sum += term;
    }
    return sum + (seq_t)1;              /* a(n) = 1 + Σ ceil(k * LOG9_6) */
}
