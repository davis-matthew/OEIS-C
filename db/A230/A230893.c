/*
 * A230893  Number of n X 2 0..3 arrays x(i,j) with each element horizontally
 *          or vertically next to at least one element with value (x(i,j)+1)
 *          mod 4 and at least one element with value (x(i,j)-1) mod 4, no
 *          adjacent elements equal, and upper left element zero.
 * https://oeis.org/A230893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 2*a(n-3); initial values: a(1) = 0;
 *            a(2) = 2; a(3) = 2
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* offset 1 – a(1) is the first term */
    if (n == 0) return (seq_t)0;      /* undefined offset, safe return */

    /* handle small indices directly */
    switch (n) {
        case 1: return (seq_t)0;
        case 2: return (seq_t)2;
        case 3: return (seq_t)2;
    }

    /* compute forward using a sliding window – O(n) time, O(1) memory */
    seq_t a_nm3 = (seq_t)0;   /* a(1)   */
    seq_t a_nm2 = (seq_t)2;   /* a(2)   */
    seq_t a_nm1 = (seq_t)2;   /* a(3)   */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)2 * a_nm1 + (seq_t)3 * a_nm2 - (seq_t)2 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = ai;
    }

    return a_nm1;
}
