/*
 * A169650  a(n) = 3*a(n-1) + 2^n - 6, with a(1) = 3.
 * https://oeis.org/A169650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*3^(n-1) - 2^(n+1) + 3. Recurrence: a(1)=3,
 *            a(n)=3*a(n-1)+2^n-6 for n>1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* offset is 1 – behaviour for n<1 is irrelevant */

    /* 3^(n-1) and 2^(n+1) */
    seq_t p3 = pow_uint(3, n > 0 ? n - 1 : 0);
    seq_t p2 = pow_uint(2, n + 1);

    /* a(n) = 4*3^(n-1) - 2^(n+1) + 3 */
    seq_t res = 4 * p3;
    res -= p2;
    res += 3;
    return res;
}
