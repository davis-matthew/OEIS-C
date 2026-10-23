/*
 * A040893  Continued fraction for sqrt(924).
 * https://oeis.org/A040893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients of sqrt(924).  
   a(0) = 30, then periodic with block {2,1,1,14,1,1,2,60}. */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)30;

    /* Periodic part of the continued fraction of sqrt(924). */
    static const seq_t block[8] = {
        (seq_t)2,  (seq_t)1,  (seq_t)1,  (seq_t)14,
        (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)60
    };

    /* n>0: compute offset within the period. */
    idx_t idx = (n - 1) % (idx_t)8;
    return block[(size_t)idx];
}
