/*
 * A183862  a(n) = n + floor(sqrt(5*n/2)); complement of A183863.
 * https://oeis.org/A183862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(5*n/2))
 * Range:     u8 n <= 25, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* n has to be >= 1 (offset 1). */
seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;           /* cast index */
    seq_t t;                      /* floor(5*n/2) */
    if ((n & 1) == 0) {
        /* even n: 5*n/2 = (n/2)*5 */
        t = (m / 2) * (seq_t)5;
    } else {
        /* odd n: 5*n/2 = (n-1)/2*5 + 2 */
        t = ((m - 1) / 2) * (seq_t)5 + (seq_t)2;
    }
    seq_t s = floor_sqrt(t);
    return m + s;
}
