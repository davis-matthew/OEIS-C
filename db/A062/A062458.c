/*
 * A062458  Nearest integer to (n+1)^(n+1)/n^n.
 * https://oeis.org/A062458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round( e * ( n + 1/2 - 1/(24n) ) ), a(0)=1
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Use the asymptotic formula from the OEIS entry.
       The approximation error is O(1/n^2), which is far below the 0.5
       rounding threshold for all n ≥ 1 up to the range needed.    */
    double dn = (double)n;
    double val = exp(1.0) * (dn + 0.5 - 1.0/(24.0 * dn));
    return (seq_t)llround(val);
}
