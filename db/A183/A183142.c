/*
 * A183142  Beatty sequence for 2/(3+sqrt(3)).
 * https://oeis.org/A183142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2*n/(3+sqrt(3)))
 * Range:     u8 n <= 6, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/bitlen_seq_t.h"

/* integer square root: floor(sqrt(x)) for any unsigned seq_t */
static seq_t isqrt_seq(seq_t x)
{
    if (x < 2) return x;
    size_t bl = bitlen_seq_t(x);          /* number of bits needed for x   */
    size_t bitpos = bl & ~ (size_t)1;     /* largest even bit position <= bl-1 */
    seq_t bit = ((seq_t)1) << bitpos;     /* highest power of 4 <= x        */
    seq_t res = 0;
    while (bit != 0) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1; behaviour for n<1 not required */

    seq_t sn = (seq_t)n;                     /* cast index to seq_t */
    seq_t x  = (seq_t)3 * sn * sn;           /* 3*n*n */
    seq_t s  = isqrt_seq(x);                /* floor(sqrt(3*n*n)) = floor(n*sqrt(3)) */
    seq_t m  = s / (seq_t)3;                /* floor(n*sqrt(3)/3) */
    return sn - (seq_t)1 - m;               /* a(n) = n-1 - floor(n*sqrt(3)/3) */
}
