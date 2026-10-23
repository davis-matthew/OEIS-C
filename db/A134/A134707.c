/*
 * A134707  a(n) = floor(n^sqrt(Pi)).
 * https://oeis.org/A134707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^sqrt(Pi))
 * Range:     u8 n <= 22, u16 n <= 521, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static const long double sqrt_pi =
    1.772453850905516027298167451933L;  /* sqrt(pi) to high precision */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                         /* n < 1 is out of OEIS range */
    /* Compute n^sqrt(pi) in long double for high precision,
       then take the floor and convert to seq_t. */
    long double val = powl((long double)n, sqrt_pi);
    return (seq_t)((unsigned long long)floorl(val));
}
