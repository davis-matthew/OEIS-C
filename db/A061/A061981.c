/*
 * A061981  a(n) = 3^n - 2*n - 1.
 * https://oeis.org/A061981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 3^n - 2*n - 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the nth term of sequence A061981. */
seq_t a(idx_t n)
{
    /* Compute 3^n */
    seq_t pow3 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
    {
        pow3 *= (seq_t)3;
    }

    /* Compute 2*n as seq_t */
    seq_t two_n = (seq_t)n * (seq_t)2;

    /* Return 3^n - 2*n - 1 */
    return pow3 - two_n - (seq_t)1;
}
