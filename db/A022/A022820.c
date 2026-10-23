/*
 * A022820  [ n/1 ] - [ (n-1)/2 ] + [ (n-2)/3 ] - ... + ((-1)^n)[ 2/(n-1) ].
 * https://oeis.org/A022820
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [ n/1 ] - [ (n-1)/2 ] + [ (n-2)/3 ] - ... + ((-1)^n)[
 *            2/(n-1) ]
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2)           /* offset is 2; lower indices are undefined */
        return 0;

    seq_t sum = 0;
    for (idx_t m = 1; m <= n - 1; ++m) {
        seq_t term = (seq_t)(n - m + 1) / (seq_t)m;  /* floor division of positive integers */
        if ((m - 1) & 1)
            sum -= term;   /* negative sign for even m (m-1 odd) */
        else
            sum += term;   /* positive sign for odd m (m-1 even) */
    }
    return sum;
}
