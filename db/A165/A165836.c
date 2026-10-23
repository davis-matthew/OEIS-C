/*
 * A165836  Totally multiplicative sequence with a(p) = 15.
 * https://oeis.org/A165836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 15^bigomega(n) = 15^(total number of prime factors of n,
 *            counted with multiplicity)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* bigomega returns the number of prime factors of n (with multiplicity) */
    seq_t w = bigomega(n);

    /* compute 15^w using the provided big_pow helper */
    return big_pow((seq_t)15, w);
}
