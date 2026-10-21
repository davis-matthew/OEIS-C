/*
 * A206529  7^n concatenated with itself.
 * https://oeis.org/A206529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of 7^n with itself
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   digit_len, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the helper routines pow_seq and digit_len from the utils library. */
#include "../utils/pow_seq.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* Compute 7^n */
    seq_t p = pow_seq((seq_t)7, (int)n);

    /* Number of decimal digits of p */
    int d = digit_len(p);

    /* 10^d */
    seq_t pow10 = pow_seq((seq_t)10, d);

    /* Concatenate: p * 10^d + p */
    return p * pow10 + p;
}
