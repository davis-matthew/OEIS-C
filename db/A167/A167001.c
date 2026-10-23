/*
 * A167001  Least possible nonnegative coefficients of x^n in G(x)^(2^n),
 *          n>=0, for an integer series G(x) such that G'(0)=G(0)=1; the G(x)
 *          that satisfies this condition equals the g.f. of A167000.
 * https://oeis.org/A167001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167001:
   a(0) = 1;
   a(n) = 0                  if n > 0 and n is not a power of two;
   a(2^m) = 2^(2^m - m)      for m >= 0 (i.e. for powers of two). */
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    /* check if n is a power of two */
    if ((n & (n - 1)) != 0) return (seq_t)0;

    /* compute m = log2(n) */
    idx_t m = 0, t = n;
    while (t > 1) {
        t >>= 1;
        ++m;
    }

    /* exponent of two: 2^n - n = 2^m - m (but 2^m == n) */
    idx_t exp = n - m;
    return pow2(exp);
}
