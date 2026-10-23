/*
 * A158010  a(n) = 256*n^2 - n.
 * https://oeis.org/A158010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 256*n^2 - n
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 4096, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 
   Computes the OEIS sequence A158010:
   a(n) = 256 * n^2 - n, for n >= 1.
   The function uses the type seq_t for the sequence value
   and idx_t for the index.  The result is correct for any n
   whose value fits in seq_t.  For larger n, the arithmetic
   is performed in the natural precision of seq_t.
*/
seq_t a(idx_t n)
{
    /* Offset is 1; for n==0 the function returns 0 (irrelevant) */
    if (n == 0)
        return (seq_t)0;

    seq_t nn = (seq_t)n;
    return (seq_t)256 * nn * nn - nn;
}
