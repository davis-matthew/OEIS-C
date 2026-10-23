/*
 * A048513  a(n) = T(8,n), array T given by A048505.
 * https://oeis.org/A048513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+33*n+256)*2^(n-2)-63 (offset 0)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 23, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the cases n = 0 and n = 1 explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)82;

    /* For n >= 2 use the closed formula */
    seq_t nn = (seq_t)n;
    seq_t t  = nn * nn + (seq_t)33 * nn + (seq_t)256;
    seq_t pow = pow2(n - 2);   /* 2^(n-2) in seq_t */
    return t * pow - (seq_t)63;
}
