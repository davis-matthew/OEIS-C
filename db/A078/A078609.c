/*
 * A078609  Least positive integer x such that 2*x^n>(x+3)^n.
 * https://oeis.org/A078609
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling( 3 / ( 2^(1/n) – 1 ) )
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 2 */

#include <math.h>
#include <stdint.h>

/* Compute a(n) for n ≥ 2.  For n < 2 the return value is irrelevant. */
seq_t a(idx_t n)
{
    /* Return a meaningless value for indices below the offset. */
    if (n < 2)
        return 0;

    /*  Compute the real‑valued expression using long double to avoid
        overflow and to keep the error far below 1.0.  The cast to
        unsigned long long happens only after the ceil operation, so
        we stay within the limits of the builtin integer type. */
    long double invroot = powl(2.0L, 1.0L / (long double)n) - 1.0L;
    long double val     = 3.0L / invroot;
    unsigned long long x = (unsigned long long)ceill(val - 1e-15L);

    /* Convert the 64‑bit result to the generic sequence type.  The
       constructor for the arbitrary‑precision seq_t accepts an
       unsigned long long, so this cast works for all builds. */
    return (seq_t)x;
}
