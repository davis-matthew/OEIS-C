/*
 * A228212  Number of second differences of arrays of length n + 2 of numbers
 *          in 0..2.
 * https://oeis.org/A228212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n+2) - 2^(n+2) for n > 3; initial terms: a(1)=9,
 *            a(2)=49, a(3)=199
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)49;
    if (n == 3) return (seq_t)199;
    idx_t exp = n + 2;              /* exponent common to both powers */
    seq_t pow3 = seq_pow((seq_t)3, exp);
    seq_t pow2 = seq_pow((seq_t)2, exp);
    return pow3 - pow2;
}
