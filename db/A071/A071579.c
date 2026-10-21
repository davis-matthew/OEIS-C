/*
 * A071579  a(n) = 2*a(n-1)*A002812(n-1), starting a(0)=1.
 * https://oeis.org/A071579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)*(6*a(n-2)^2 + 1) with a(0)=1, a(1)=4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the base cases */
    if (n == (idx_t)0)
        return (seq_t)1;
    if (n == (idx_t)1)
        return (seq_t)4;

    /* iterate with a(n-2) and a(n-1) */
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)4;   /* a(1) */
    seq_t a_n   = a_nm1;      /* placeholder for current a(n) */

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        seq_t temp   = 6 * a_nm2 * a_nm2;   /* 6*a(n-2)^2 */
        temp = temp + (seq_t)1;              /* +1 */
        a_n = 2 * a_nm1 * temp;              /* 2*a(n-1)*(6*a(n-2)^2 + 1) */

        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
