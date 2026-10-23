/*
 * A027626  Denominator of n*(n+5)/((n+2)*(n+3)).
 * https://oeis.org/A027626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of (n+2)*(n+3)/6 * Equivalent to a(n) =
 *            ((n+2)*(n+3)) / gcd( (n+2)*(n+3) , 6 )
 * Range:     u8 n <= 13, u16 n <= 262, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t prod = (n_seq + 2) * (n_seq + 3);
    seq_t d = gcd(prod, (seq_t)6);
    return prod / d;
}
