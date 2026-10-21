/*
 * A204238  Determinant of the n-th principal submatrix of A204237.
 * https://oeis.org/A204238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 2, a(2) = -17, a(n) = -8*a(n-1) -16*a(n-2), n >= 3
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 14, i64 n <= 29, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)-17;

    seq_t a_nm2 = (seq_t)2;      /* a(n-2)  */
    seq_t a_nm1 = (seq_t)-17;    /* a(n-1)  */
    for (idx_t k = 3; k <= n; ++k)
    {
        /* a(n) = -8*a(n-1) - 16*a(n-2) */
        seq_t a_n = ((seq_t)-8) * a_nm1 - ((seq_t)16) * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
