/*
 * A080420  a(n) = (n+1)*(n+6)*3^n/6.
 * https://oeis.org/A080420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)*(n+6)*3^n/6 (OEIS A080420)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   _pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_pow3.h"

seq_t a(idx_t n)
{
    /* Offset 0: a(0) = 1, handled explicitly. */
    if (n == 0U)
        return 1;

    /* Re‑write a(n) for n ≥ 1 as
       a(n) = ((n+1)*(n+6)/2) * 3^(n-1)   (since 3^n/6 = 3^(n-1)/2).   */
    seq_t prod = (seq_t)(n + 1) * (seq_t)(n + 6);   /* (n+1)*(n+6) is always even */
    prod /= (seq_t)2;                               /* safe integer division */

    return prod * _pow3(n - 1U);
}
