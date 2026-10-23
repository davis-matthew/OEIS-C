/*
 * A165865  Totally multiplicative sequence with a(p) = 44.
 * https://oeis.org/A165865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 44^bigomega(n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 63, u64 n <= 4095,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* bigomega(n) returns the total number of prime factors of n
       (counted with multiplicity).  We then raise 44 to that power. */
    seq_t w = bigomega(n);
    return big_pow((seq_t)44, w);
}
