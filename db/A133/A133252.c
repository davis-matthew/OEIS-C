/*
 * A133252  Partial sums of A006000.
 * https://oeis.org/A133252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)*(n+2)*(3*n^2+5*n+12)/24
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t n1   = (seq_t)n;
    seq_t a1   = n1 + 1;            /* n+1 */
    seq_t b1   = n1 + 2;            /* n+2 */
    seq_t c1   = 3 * n1 * n1 + 5 * n1 + 12; /* 3*n^2 + 5*n + 12 */

    seq_t ab   = a1 * b1;           /* (n+1)*(n+2) */
    seq_t d1   = gcd(ab, (seq_t)24); /* reduce to avoid overflow */
    seq_t part = ab / d1;
    seq_t d2   = (seq_t)24 / d1;
    seq_t res  = part * c1 / d2;    /* final result */

    return res;
}
