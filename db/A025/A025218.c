/*
 * A025218  a(n) = floor( Sum_{k=1..n} sqrt(k+1) ).
 * https://oeis.org/A025218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Σ_{k=1}^{n} sqrt(k+1) )
 * Range:     u8 n <= 51, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    double s = 0.0;
    for (idx_t k = 1; k <= n; ++k)
        s += sqrt((double)k + 1.0);

    /* Convert the floating‑point result to an integer.  The cast to
       unsigned long long guarantees an integral value; the outer
       cast to seq_t allows the conversion to an arbitrary‑precision
       integer type when seq_t is not a fixed‑width integer. */
    return (seq_t)(unsigned long long)floor(s);
}
