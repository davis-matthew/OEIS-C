/*
 * A165862  Totally multiplicative sequence with a(p) = 41.
 * https://oeis.org/A165862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 41^bigomega(n)
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
    if (n < 1) return 0;          /* n is defined for n >= 1 */
    seq_t omega = bigomega(n);    /* total number of prime factors (with multiplicity) */
    return big_pow((seq_t)41, omega);  /* 41 raised to that power */
}
