/*
 * A121253  a(n) = a(n-1)*a(n-3)+1 with a(0)=a(1)=a(2)=0.
 * https://oeis.org/A121253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*a(n-3)+1, a(0)=a(1)=a(2)=0
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 15, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    seq_t a0 = 0, a1 = 0, a2 = 0, a3 = 0;
    for (idx_t i = 3; i <= n; ++i) {
        a3 = a2 * a0 + 1;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a3;
}
