/*
 * A027471  a(n) = (n-1)*3^(n-2), n > 0.
 * https://oeis.org/A027471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*3^(n-2), for n > 0 (a(1)=0)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    /* compute (n-1) * 3^(n-2) */
    seq_t base = (seq_t)(n - 1);
    seq_t pw   = pow3(n - 2);
    return base * pw;
}
