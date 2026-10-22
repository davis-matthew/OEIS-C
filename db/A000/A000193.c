/*
 * A000193  Nearest integer to log n.
 * https://oeis.org/A000193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(log(n)), i.e. the k with exp(k-1/2) <= n <
 *            exp(k+1/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* log is taken of the index only (a native integer), and the result is
       small (at most 45 for 64-bit n), so long double precision is enough.
       exp(k+1/2) is never an integer, so there is no exact tie to round. */
    long double x = logl((long double)n);
    long long k = (long long)floorl(x + 0.5L);
    if (k < 0)
        k = 0;
    return (seq_t)k;
}
