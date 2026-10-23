/*
 * A190279  a(n) = n + [nr/s] + [nt/s]; r=1/2, s=sinh(1), t=cosh(1).
 * https://oeis.org/A190279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊(n/2)*csch(1)⌋ + ⌊n*coth(1)⌋
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Here csch(1)=1/sinh(1) and coth(1)=cosh(1)/sinh(1).  */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precompute the hyperbolic constants only once. */
    static long double sinh1 = 0.0L;
    static long double cosh1 = 0.0L;
    if (sinh1 == 0.0L) {
        long double e   = expl(1.0L);
        long double en  = expl(-1.0L);
        sinh1 = (e - en) * 0.5L;          /* sinh(1) */
        cosh1 = (e + en) * 0.5L;          /* cosh(1) */
    }

    /* Compute the two floor terms using long double arithmetic. */
    long double term1 = floorl(((long double)n) * 0.5L / sinh1);
    long double term2 = floorl(((long double)n) * cosh1 / sinh1);

    /* Convert floor results to unsigned 64‑bit integers (they fit). */
    uint64_t f1 = (uint64_t)term1;
    uint64_t f2 = (uint64_t)term2;

    /* Assemble the result, converting the native integers to seq_t. */
    seq_t result = (seq_t)n + (seq_t)f1 + (seq_t)f2;
    return result;
}
