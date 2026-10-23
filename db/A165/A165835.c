/*
 * A165835  Totally multiplicative sequence with a(p) = 14.
 * https://oeis.org/A165835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 14^Ω(n)
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
    if (n == 0) return (seq_t)0;          /* undefined below offset, placeholder */
    seq_t omega = bigomega(n);             /* Ω(n): total number of prime factors (with multiplicity) */
    return big_pow((seq_t)14, omega);     /* 14^Ω(n) */
}
