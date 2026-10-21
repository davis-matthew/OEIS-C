/*
 * A087206  a(n) = 2*a(n-1) + 4*a(n-2); with a(0)=1, a(1)=4.
 * https://oeis.org/A087206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 4; a(n) = 2*a(n-1) + 4*a(n-2) for n >= 2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)4;   /* a(n-1) */
    seq_t a_n   = 0;          /* current a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        a_n = a_nm1 * (seq_t)2 + a_nm2 * (seq_t)4;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
