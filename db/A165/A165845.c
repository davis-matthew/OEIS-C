/*
 * A165845  Totally multiplicative sequence with a(p) = 24.
 * https://oeis.org/A165845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 24^bigomega(n) (bigomega counts prime factors with
 *            multiplicity)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/bigomega.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* n ≥ 1 by offset.  bigomega returns the count of prime factors
       with multiplicity as a seq_t; cast to idx_t for seq_pow. */
    idx_t exp = (idx_t)bigomega(n);
    return seq_pow((seq_t)24, exp);
}
