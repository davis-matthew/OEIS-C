/*
 * A079554  a(n) = floor( d(n) * log(n) / 2 ), where d() = A000005.
 * https://oeis.org/A079554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( d(n) * log(n) / 2 )
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* d(n) is the divisor-count function A000005. */
#include <math.h>
#include <stddef.h>
#include <stdint.h>

/* Sequence domain and codomain types are supplied by the build system. */
seq_t a(idx_t n)
{
    /* Guard against out-of-range indices; the OEIS offset is 1. */
    if (n < 1) return (seq_t)0;

    /* Compute d(n) by trial division. */
    idx_t temp   = n;          /* copy of n for factorisation */
    unsigned long long d = 1;   /* divisor count (never exceeds 64 bits for n ~ 1e9) */

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            unsigned long long exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            d *= (exp + 1);
        }
    }
    if (temp > 1)  /* remaining prime factor */
        d *= 2;

    /* Compute floor( d * log(n) / 2 ) using double arithmetic
       and cast the integer result to seq_t. */
    double val = (double)d * log((double)n) / 2.0;
    return (seq_t)( (unsigned long long)(val) );
}
