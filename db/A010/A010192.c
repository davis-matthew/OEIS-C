/*
 * A010192  Continued fraction for sqrt(133).
 * https://oeis.org/A010192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: continued fraction coefficients for sqrt(133). 
   Periodic with period 16 starting from index 1. 
   a(0)=11; for n>=1, a(n)=pattern[(n-1)%16]. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)11;

    static const seq_t pattern[16] = {
        (seq_t)1, (seq_t)1, (seq_t)7, (seq_t)5, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)2,
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)7, (seq_t)1, (seq_t)1, (seq_t)22
    };

    idx_t idx = (n - 1) % (idx_t)16;
    return pattern[idx];
}
