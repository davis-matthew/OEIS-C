/*
 * A099018  Duplicate of A033484.
 * https://oeis.org/A099018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 3*2^n - 2
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 23, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(0) = 1, a(n) = 2*a(n-1) + 2 for n > 0 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned __int128 r = 1;              /* a(0) = 1 */
    for (idx_t i = 0; i < n; ++i) {
        r = r * 2 + 2;                   /* a(i+1) = 2*a(i) + 2 */
    }
    return (seq_t)r;
}
