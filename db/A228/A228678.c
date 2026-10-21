/*
 * A228678  Number of nX3 binary arrays with no two ones adjacent
 *          horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) - 3*a(n-3) with a(1)=5, a(2)=19,
 *            a(3)=77
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == (idx_t)1) return (seq_t)5;
    if (n == (idx_t)2) return (seq_t)19;
    if (n == (idx_t)3) return (seq_t)77;

    /* a(n-3), a(n-2), a(n-1) for the current step */
    seq_t a_nm3 = (seq_t)5;   /* a(1)   */
    seq_t a_nm2 = (seq_t)19;  /* a(2)   */
    seq_t a_nm1 = (seq_t)77;  /* a(3)   */

    /* Iterate from n=4 up to the required n */
    for (idx_t i = (idx_t)4; i <= n; ++i)
    {
        seq_t a_n = 5 * a_nm1 - 3 * a_nm2 - 3 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
