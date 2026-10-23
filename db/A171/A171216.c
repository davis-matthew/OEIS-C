/*
 * A171216  a(n) = (4^(5*n+1) + 7)/11.
 * https://oeis.org/A171216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^(5*n+1) + 7) / 11
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS offset is 1 – a(1) is the first term. */
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* n >= 1  (the function is not defined for n < 1, but callers
       always use n >= 1 according to the problem statement). */
    if (n < 1) return (seq_t)0;

    /* Compute the exponent 5*n + 1.  For all intended n this fits
       comfortably in idx_t. */
    idx_t exp = 5 * n + 1;

    /* Compute 4^exp.  seq_pow gives an exact integer power, using
       the proper representation of seq_t. */
    seq_t pow4 = seq_pow((seq_t)4, exp);

    /* Add 7 and divide by 11.  All operations are exact for the
       given types. */
    seq_t num = pow4 + (seq_t)7;
    seq_t res = num / (seq_t)11;

    return res;
}
