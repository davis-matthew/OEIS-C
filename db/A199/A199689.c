/*
 * A199689  a(n) = 8*10^n + 1.
 * https://oeis.org/A199689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*10^n + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the helper that computes 10^e for arbitrary precision or
   small-width types. */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n using the provided utility. */
    seq_t val = pow10(n);

    /* Multiply by 8 and add 1.  Constants are converted to seq_t
       implicitly, but casting does not hurt and keeps the intent
       clear. */
    val = val * (seq_t)8;
    val += (seq_t)1;

    return val;
}
