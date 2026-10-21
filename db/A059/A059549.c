/*
 * A059549  Beatty sequence for 1 + 1/log(10).
 * https://oeis.org/A059549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * (1 + 1 / log(10)))
 * Range:     u8 n <= 178, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

#define ALPHA ((long double)1.0 + (long double)1.0 / logl((long double)10.0))

seq_t a(idx_t n)
{
    /* For n == 0 (outside the actual offset) return 0. */
    if (n == 0) return 0;

    /* Compute using long double arithmetic and cast back to seq_t. */
    long double val = (long double)n * ALPHA;
    return (seq_t)(unsigned long long)floorl(val);
}
