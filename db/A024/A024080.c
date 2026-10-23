/*
 * A024080  a(n) = 7^n - n^5.
 * https://oeis.org/A024080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - n^5
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Compute 7^n */
    seq_t pow7 = pow_seq((seq_t)7, (int)n);
    /* Compute n^5 */
    seq_t pow_n5 = pow_seq((seq_t)n, 5);
    /* Return the difference */
    return pow7 - pow_n5;
}
