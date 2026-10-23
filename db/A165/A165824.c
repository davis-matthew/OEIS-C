/*
 * A165824  Totally multiplicative sequence with a(p) = 3.
 * https://oeis.org/A165824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 3^bigomega(n)
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    /* bigomega(n) returns the number of prime factors of n counting multiplicity.
       pow3(exp) returns 3^exp as seq_t. */
    seq_t omega = bigomega(n);
    return pow3((idx_t)omega);
}
