/*
 * A227042  Triangle of denominators of harmonic mean of n and m, 1 <= m <= n.
 * https://oeis.org/A227042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, tabl
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) is the n-th entry in the flattened triangle of denominators
   of the harmonic mean H(n,m)=2*n*m/(n+m) for 1≤m≤n.
   For an integer n (offset 1), let r and c be the unique indices
   such that 1+2+⋯+(r−1) < n ≤ 1+2+⋯+r and c = n−((r−1)·r)/2.
   Then a(n) = (r+c)/gcd(2·r·c, r+c).
*/

#include <math.h>
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* find the row r (1-indexed) such that r(r+1)/2 ≥ n
       and (r−1)r/2 < n.                              */
    idx_t r = (idx_t)((sqrt(8.0 * n + 1.0) - 1.0) / 2.0);
    if (r == 0) r = 1;                                 /* safety for n=1 */
    while (r * (r + 1) / 2ULL < n) ++r;
    while (r > 1 && (r - 1) * r / 2ULL >= n) --r;

    /* column index c */
    idx_t c = n - (r - 1) * r / 2ULL;

    /* compute denominator (r+c)/gcd(2*r*c, r+c) */
    seq_t denom = (seq_t)(r + c);
    seq_t num   = 2ull * (seq_t)r * (seq_t)c;
    seq_t g     = seq_gcd(num, denom);

    return denom / g;
}
