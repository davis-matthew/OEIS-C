/*
 * A176029  a(n)= n^Omega(n).
 * https://oeis.org/A176029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^A001222(n) (n^Omega(n))
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 63, u64 n <= 255,
 *            u128 n <= 4095, big any n
 * Helpers:   bigomega, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t om = bigomega(n);          /* number of prime factors of n (with multiplicity) */
    return seq_pow((seq_t)n, (idx_t)om);  /* n^om */
}
