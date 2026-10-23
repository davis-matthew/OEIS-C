/*
 * A133335  a(3*n) = 3*a(3*n-1)-3*a(3*n-2)+2*a(3*n-3), a(3*n+1) =
 *          3*a(3*n)-3*a(3*n-1)+2*a(3*n-2), a(3*n+2) = 3*a(3*n+1)-3*a(3*n)
 *          with a(0)=1, a(1)=2, a(2)=3.
 * https://oeis.org/A133335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-3) (a(0)=1, a(1)=2, a(2)=3)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"

seq_t a(idx_t n)
{
    static const seq_t base[3] = { (seq_t)1, (seq_t)2, (seq_t)3 };
    idx_t rem = n % 3;
    idx_t exp = n / 3;
    return base[rem] * power5(exp);
}
