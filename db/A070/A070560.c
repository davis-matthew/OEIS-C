/*
 * A070560  a(0) = 1; for n > 0, a(n) = (fecundity of n) + 2.
 * https://oeis.org/A070560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   fecundity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = fecundity(n) + 2, where "fecundity" is the number of
   transitions minus one in the process defined in OEIS A070560:
   start with x = n. While x ≠ 0, let p be the product of the decimal
   digits of x. If p = 0 the process stops (x is set to 0). Otherwise
   replace x by x + p and continue.  The fecundity is the number of
   transitions before reaching 0, minus one.  The special case is a(0)=1. */

#include <stdint.h>
#include "../utils/fecundity.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 1;                   /* a(0) is defined as 1   */
    return fecundity(n) + 2;
}
