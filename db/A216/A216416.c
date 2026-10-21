/*
 * A216416  a(n) = smallest m such that 2n-1 | 10^m+1, or 0 if no such m
 *          exists.
 * https://oeis.org/A216416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = smallest m such that 2*n-1 divides 10^m + 1,
 *              or 0 if no such m exists.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t d = ((seq_t)n) * 2 - 1;   /* modulus 2*n-1 */

    /* If d is a multiple of 5, 10^m + 1 cannot be divisible by d */
    if (d % 5 == 0) return 0;

    seq_t x = 10 % d;   /* current 10^k mod d, k starts at 1 */
    seq_t m = 1;        /* current exponent */

    /* Handle the trivial case d == 1 (then 10^1 + 1 is divisible). */
    if (x == d - 1) return m;   /* 10^1 ≡ -1 (mod d)  */
    if (x == 1)     return 0;   /* cycle returned to 1 before -1 */

    /* Iterate powers of 10 modulo d until we find -1 or return to 1. */
    for (;;) {
        ++m;
        x = (x * 10) % d;
        if (x == d - 1) return m;   /* found 10^m ≡ -1 (mod d) */
        if (x == 1)      return 0;   /* cycle completed without -1 */
    }
}
