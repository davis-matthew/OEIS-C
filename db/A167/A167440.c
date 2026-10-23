/*
 * A167440  5th GegenbauerC polynomial evaluated at powers of 2 (multiplied by
 *          5).
 * https://oeis.org/A167440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * (5 - 5*4^n + 16^n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t pow2  = seq_pow((seq_t)2,  n);
    seq_t pow4  = seq_pow((seq_t)4,  n);
    seq_t pow16 = seq_pow((seq_t)16, n);

    seq_t term = (seq_t)5;
    term -= (seq_t)5 * pow4;
    term += pow16;

    return pow2 * term;
}
