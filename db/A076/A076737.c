/*
 * A076737  Let u(1)=u(2)=u(3)=2, u(n)=(1+u(n-1)u(n-2))/u(n-3); then a(n) is
 *          the numerator of u(n).
 * https://oeis.org/A076737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(1)=a(2)=a(3)=2; for n≥4, a(n)=1+2^(n‑2) if n even,
 *            a(n)=(1+2^(n‑2))/3 if n odd
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1 || n == 2 || n == 3)
        return (seq_t)2;

    /* n ≥ 4 */
    seq_t p = pow2(n - 2);     /* p = 2^(n-2) */
    seq_t res = p + 1;         /* 1 + 2^(n-2) */

    if (n & 1)                /* odd n → divide by 3 */
        res = res / 3;

    return res;
}
