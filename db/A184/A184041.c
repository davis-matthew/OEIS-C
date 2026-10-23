/*
 * A184041  1/9 the number of (n+1) X 3 0..2 arrays with all 2 X 2 subblocks
 *          having the same four values.
 * https://oeis.org/A184041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*2^(n/2)+2^(n+1)+7 for n even * a(n) =
 *            2^(n+1)+2^((n+3)/2)+7 for n odd * (OEIS A184041)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n % 2 == 0) {                 /* n even */
        seq_t t1 = pow2(n / 2);        /* 2^(n/2) */
        t1 = t1 * 3;                   /* 3*2^(n/2) */
        seq_t t2 = pow2(n + 1);        /* 2^(n+1) */
        return t1 + t2 + 7;            /* sum */
    } else {                           /* n odd */
        seq_t t1 = pow2(n + 1);        /* 2^(n+1) */
        seq_t t2 = pow2((n + 3) / 2);  /* 2^((n+3)/2) */
        return t1 + t2 + 7;            /* sum */
    }
}
