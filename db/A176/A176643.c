/*
 * A176643  Triangle T(n, k) = 21^(k*(n-k)), read by rows.
 * https://oeis.org/A176643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 21^(k*(r - k)), where r = index of the row containing n,
 *            computed as the smallest integer r with r*(r+1)/2 > n; k = n -
 *            r*(r+1)/2 is the column within that row
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 38, u128 n <= 70,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 > n,
       and cum holds the number of terms before this row. */
    idx_t r = 0;
    idx_t cum = 0;
    for (;; ++r) {
        idx_t next = cum + (r + 1);   /* terms up to this row */
        if (next > n)
            break;
        cum = next;
    }

    idx_t k = n - cum;                      /* column index */
    seq_t exp = (seq_t)k * (seq_t)(r - k);   /* exponent */

    if (exp == 0)
        return (seq_t)1;                    /* 21^0 = 1 */

    seq_t base = 21;
    return big_pow(base, exp);
}
