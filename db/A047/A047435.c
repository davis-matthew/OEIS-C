/*
 * A047435  Numbers that are congruent to {1, 2, 4, 5, 6} mod 8.
 * https://oeis.org/A047435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + {1,2,4,5,6}[(n-1) mod 5]
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(1) = 1, a(2) = 2, a(3) = 4, a(4) = 5, a(5) = 6, ...   */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined below offset */
        return 0;

    idx_t m = (n - 1) / 5;    /* number of complete 5-block groups */
    idx_t r = (n - 1) % 5;    /* position inside the current group   */

    static const seq_t offs[5] = {1, 2, 4, 5, 6};

    /* 8 * m fits in idx_t comfortably; cast to seq_t afterwards.
        If seq_t cannot hold the result, truncation is acceptable
        because the value then exceeds the capacity of the type.   */
    seq_t base = (seq_t)(m * 8u);

    return base + offs[r];
}
