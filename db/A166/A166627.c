/*
 * A166627  Totally multiplicative sequence with a(p) = 6p for prime p.
 * https://oeis.org/A166627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * 6^bigomega(n)
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
    /* Compute 6^Ω(n) where Ω(n) is the number of prime factors of n
       counting multiplicity.  bigomega(n) returns Ω(n) as seq_t. */
    seq_t pow6 = big_pow((seq_t)6, bigomega(n));

    /* Multiply by n in seq_t arithmetic. */
    return (seq_t)n * pow6;
}
