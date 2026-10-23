/*
 * A165833  Totally multiplicative sequence with a(p) = 12.
 * https://oeis.org/A165833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 12^bigomega(n)
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
    if (n < 1) return 0;          /* undefined for n < 1, but safe to return 0 */
    seq_t w = bigomega(n);        /* total number of prime factors, counted with multiplicity */
    return big_pow(12, w);        /* 12 to the power of w */
}
