/*
 * A165849  Totally multiplicative sequence with a(p) = 28.
 * https://oeis.org/A165849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 28^bigomega(n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, safety */
    seq_t w = bigomega(n);          /* total prime factors (with multiplicity) */
    return big_pow((seq_t)28, w);   /* 28^w */
}
