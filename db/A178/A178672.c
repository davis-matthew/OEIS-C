/*
 * A178672  a(n) = 6^n - 6.
 * https://oeis.org/A178672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - 6 Offset: 1 (a(1) is the first term listed)
 *            Dependencies: seq_pow from utils/seq_pow.h
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute 6^n using the generic power routine and subtract 6.
       This works for all n >= 1 and uses only seq_t arithmetic. */
    seq_t pow6 = seq_pow((seq_t)6, n);
    return pow6 - ((seq_t)6);
}
