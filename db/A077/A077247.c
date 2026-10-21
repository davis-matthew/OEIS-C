/*
 * A077247  Combined Diophantine Chebyshev sequences A077245 and A077243.
 * https://oeis.org/A077247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-2) - a(n-4) for n >= 4, with a(0)=1, a(1)=2,
 *            a(2)=10, a(3)=17
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)17;

    /* Variables for a(i-4), a(i-3), a(i-2), a(i-1) */
    seq_t a_nm4 = (seq_t)1;   /* a(0)   */
    seq_t a_nm3 = (seq_t)2;   /* a(1)   */
    seq_t a_nm2 = (seq_t)10;  /* a(2)   */
    seq_t a_nm1 = (seq_t)17;  /* a(3)   */
    seq_t ai = 0;

    /* Compute terms iteratively up to n */
    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)8 * a_nm2 - a_nm4;  /* a(i) = 8*a(i-2) - a(i-4) */

        /* Shift the window for the next iteration */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = ai;
    }

    return ai;
}
