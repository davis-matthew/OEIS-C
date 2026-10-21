/*
 * A076191  First differences of A001222.
 * https://oeis.org/A076191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Omega(n+1) - Omega(n) (Omega = total number of prime
 *            factors, counting multiplicity)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so n >= 1 in normal usage.  For n = 0 behaviour is undefined. */
    seq_t o_n     = bigomega(n);
    seq_t o_next  = bigomega(n + 1);
    return o_next - o_n;
}
