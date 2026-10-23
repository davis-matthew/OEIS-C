/*
 * A017914  Powers of sqrt(3) rounded to nearest integer.
 * https://oeis.org/A017914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round( sqrt(3)^n ) OEIS A017914: a(2k) = 3^k, a(2k+1) =
 *            round( sqrt(3) * 3^k )
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 80, u128 n <= 160,
 *            big any n
 * Helpers:   floor_sqrt, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* even indices */
    if ((n & 1) == 0) {
        idx_t k = n >> 1;               /* n = 2k */
        return pow_int((seq_t)3, k);    /* 3^k */
    }

    /* odd indices */
    /* a(2k+1) = round( sqrt(3) * 3^k ) = round( sqrt(3^(2k+1)) ) */
    seq_t x = pow_int((seq_t)3, n);          /* 3^n, n = 2k+1 */
    seq_t y = floor_sqrt(x);                 /* floor( sqrt(x) ) */
    seq_t diff = x - y * y;                  /* x - y^2   (>=0) */
    if (diff >= y + (seq_t)1)                /* round up? */
        return y + 1;
    else
        return y;                            /* round down */
}
