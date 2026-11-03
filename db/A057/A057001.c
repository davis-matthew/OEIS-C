/*
 * A057001  (phi(n+1)-phi(n))/2.
 * https://oeis.org/A057001
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (phi(n+1) - phi(n)) / 2
 * Range:     i8 n <= 357, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9908 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* Compute φ(n) and φ(n+1) using the supplied helper
       and return half their difference.  The subtraction
       is always even, so integer division yields the exact
       value for all n ≥ 1. */
    seq_t diff = totient(n + 1) - totient(n);
    return diff / 2;
}
