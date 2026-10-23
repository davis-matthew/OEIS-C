/*
 * A165830  Totally multiplicative sequence with a(p) = 9.
 * https://oeis.org/A165830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 9^bigomega(n)
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t w = bigomega(n);      /* total number of prime factors of n (with multiplicity) */
    return big_pow((seq_t)9, w); /* compute 9^w in seq_t type */
}
