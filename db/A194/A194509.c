/*
 * A194509  Second coordinate of (2,3)-Lagrange pair for n.
 * https://oeis.org/A194509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n - 2*floor((2*n + 2)/5)
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute floor((2*n + 2)/5) safely in 128-bit arithmetic. */
    unsigned __int128 tmp = ( (unsigned __int128)n + 1 ) * 2;
    tmp /= 5;
    idx_t k = (idx_t)tmp;                       /* k = floor((2*(n+1))/5) */

    /* Compute a(n) = n - 2*k.  All operations are performed in the
     * smallest wide integer type that fits the intermediate results
     * so that the final result is correct in seq_t when it does not
     * overflow that type. */
    seq_t res = (seq_t)n;
    unsigned __int128 sub = (unsigned __int128)k * 2;  /* 2*k in 128-bit */
    res -= (seq_t)sub;
    return res;
}
