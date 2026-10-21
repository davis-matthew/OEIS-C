/*
 * A227684  Numerator of least splitting rational of log(n) and log(n+1).
 * https://oeis.org/A227684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the least splitter of log(n) and log(n+1)
 * Range:     u8 n <= 145, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The least splitter of x<y is the smallest d>0 such that ∃c with 
 *     x <= c/d < y
 * The rational c/d is the least splitting rational, and a(n) is its numerator.
 * This implementation searches for the minimum denominator d by stepping
 * from 1 upward.  For each d we compute c = ceil(d·log(n)).  If c < d·log(n+1)
 * the inequality holds and c/d is the least splitter.  The result c is
 * returned as type seq_t.  All floating‑point work uses long double
 * to keep sufficient precision; the numerator itself is converted to an
 * unsigned long long before being cast to seq_t. */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* log(n) and log(n+1) – use long double for accuracy */
    long double ln_n   = logl((long double)n);
    long double ln_np1 = logl((long double)n + 1.0L);

    for (idx_t d = 1; ; ++d) {
        /* c = ceil(d·ln(n)) */
        unsigned long long c = (unsigned long long)ceill(ln_n * (long double)d);

        /* Test the inequality c < d·ln(n+1) */
        if ((long double)c < ln_np1 * (long double)d) {
            return (seq_t)c;
        }
    }
}
