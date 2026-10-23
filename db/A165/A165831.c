/*
 * A165831  Totally multiplicative sequence with a(p) = 10.
 * https://oeis.org/A165831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 10^A001222(n) = 10^bigomega(n)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* bigomega(n) gives the total number of prime factors of n
       (counted with multiplicity).  pow10(e) returns 10^e
       as a seq_t value. */
    return pow10((idx_t)bigomega(n));
}
