/*
 * A059546  Beatty sequence for log(10)/(log(10)-1).
 * https://oeis.org/A059546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*log(10)/(log(10)-1))
 * Range:     u8 n <= 144, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Pre‑compute the irrational constant as a long double */
static const long double ALPHA = logl(10.0L) / (logl(10.0L) - 1.0L);

seq_t a(idx_t n)
{
    /* Compute the product in long double, add a tiny epsilon to guard
       against round‑off that could push the value one unit below the
       true floor. */
    long double val = (long double)n * ALPHA + 1e-12L;

    /* Convert to an integral type that the arbitrary‑precision builder
       accepts.  The class oeis_big (used when seq_t is a big integer)
       has a constructor that takes any native integral type.  The same
       holds for the built‑in unsigned integers used for smaller builds. */
    unsigned long long v = (unsigned long long)val;

    /* Cast to seq_t – this is safe for all the builds because
       seq_t can be constructed from any native integral type. */
    return (seq_t)v;
}
