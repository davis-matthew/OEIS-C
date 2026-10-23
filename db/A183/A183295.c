/*
 * A183295  Complement of A115067.
 * https://oeis.org/A183295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + m if n > 3*m*(m+1)/2 * a(n) = n + m - 1 otherwise, *
 *            where m = floor( sqrt( 2*(n-1)/3 ) )
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* offset 1: a(1) = 1.  For n==0 any value suffices for the test harness. */
    if (n == 0)
        return (seq_t)0;

    /* m = floor( sqrt( 2*(n-1)/3 ) ) */
    idx_t tmp = n - 1;
    seq_t val = (seq_t)( ( (seq_t)2 * (seq_t)tmp ) / (seq_t)3 );
    idx_t m = (idx_t)floor_sqrt(val);

    /* threshold = 3*m*(m+1)/2 */
    seq_t thr = (seq_t)3 * (seq_t)m * (seq_t)(m + 1) / (seq_t)2;

    seq_t res;
    if ( (seq_t)n > thr )
        res = (seq_t)n + (seq_t)m;
    else
        res = (seq_t)n + (seq_t)m - (seq_t)1;

    return res;
}
