/*
 * A127207  Half-indexed Lucas numbers a(n)=round(sqrt((1+sqrt(5))/2)^n)
 *          a(2n)=L(n)=A000032, so a(n)=L(n/2).
 * https://oeis.org/A127207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round( sqrt( L(n) ) ) where L(0)=2, L(1)=1 and
 *            L(k)=L(k-1)+L(k-2)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 is handled specially; for n=0 (not part of the
       defined sequence) we return 0. */
    if (n <= 1) {
        return (seq_t)(n == 0 ? 0 : 1);
    }

    /* Compute the Lucas number L(n). */
    seq_t L_nm2 = (seq_t)2; /* L(0) */
    seq_t L_nm1 = (seq_t)1; /* L(1) */
    seq_t L_n = 0;

    for (idx_t i = 2; i <= n; ++i) {
        L_n   = L_nm1 + L_nm2;
        L_nm2 = L_nm1;
        L_nm1 = L_n;
    }

    /* Round the square root of L(n). */
    seq_t s        = floor_sqrt(L_n);      /* s = ⌊√L(n)⌋            */
    seq_t sq       = s * s;                /* s²                     */
    seq_t threshold = sq + s + 1;          /* s² + s + 1             */
    if (threshold <= L_n) {
        s++;                               /* round up */
    }
    return s;
}
