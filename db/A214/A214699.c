/*
 * A214699  a(n) = 3*a(n-2) - a(n-3) with a(0)=0, a(1)=3, a(2)=0.
 * https://oeis.org/A214699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-2) - a(n-3) with a(0)=0, a(1)=3, a(2)=0
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 34, i64 n <= 69, i128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for bool, not actually needed here */

seq_t a(idx_t n)
{
    /* Return base values for the first three indices */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)0;

    /* Start with the known values a(0), a(1), a(2) */
    seq_t a_nm3 = (seq_t)0;   /* a(n-3)  */
    seq_t a_nm2 = (seq_t)3;   /* a(n-2)  */
    seq_t a_nm1 = (seq_t)0;   /* a(n-1)  */
    seq_t a_n   = (seq_t)0;   /* current a(n) */

    for (idx_t i = 3; i <= n; ++i)
    {
        a_n = (seq_t)3 * a_nm2 - a_nm3;   /* a(n) = 3*a(n-2) - a(n-3) */
        /* rotate the window: a(n-3) <- a(n-2), a(n-2) <- a(n-1), a(n-1) <- a(n) */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
