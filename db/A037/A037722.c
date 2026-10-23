/*
 * A037722  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 2,0,1,3.
 * https://oeis.org/A037722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2, a(n)=a(n-1)*10 + pat[(n-1)%4]  with pat={2,0,1,3} */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but defined for safety */
    static const unsigned int pat[4] = {2, 0, 1, 3};
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        unsigned int d = pat[(unsigned int)((i - 1) & 3UL)];  /* (i-1)%4 */
        val = val * 10 + d;
    }
    return val;
}
