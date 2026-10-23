/*
 * A158859  a(n) is formed by 2n+1 concatenations of the digit 2r+1 where n=r
 *          (mod 5).
 * https://oeis.org/A158859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit d = 2*(n mod 5)+1 repeated (2*n+1) times
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    idx_t r     = n % (idx_t)5;          /* n mod 5 */
    seq_t digit = (seq_t)(2 * r + 1);    /* 2*(n mod 5)+1, a digit 1,3,5,7,9 */
    idx_t len   = (idx_t)2 * n + 1;      /* number of digits */

    seq_t ten_pow = pow10(len);               /* 10^len */
    seq_t repunit = (ten_pow - (seq_t)1) / (seq_t)9; /* (10^len-1)/9 = 111..1 */
    return digit * repunit;                   /* d * repunit = d repeated len times */
}
