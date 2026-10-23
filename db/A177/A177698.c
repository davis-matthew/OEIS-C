/*
 * A177698  Expansion of e.g.f.: sin(arctan(x)).
 * https://oeis.org/A177698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=0; a(1)=1; a(n)=0 for even n>0; a(n) = -n*(n-2)*a(n-2) for
 *            odd n≥3
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 14, i64 n <= 22, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((n & 1) == 0) return 0;           /* even indices are zero */
    seq_t val = 1;                        /* a(1) = 1 */
    for (idx_t k = 3; k <= n; k += 2) {   /* iterate over odd indices 3,5,7,… */
        val = -( (seq_t)k * (seq_t)(k - 2) * val );
    }
    return val;
}
