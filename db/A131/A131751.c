/*
 * A131751  Numbers that are both centered triangular and centered pentagonal.
 * https://oeis.org/A131751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 62*a(n+1) - a(n) - 30 Initial values: a(1) = 1, a(2) =
 *            31 Computes terms that are both centered triangular and centered
 *            pentagonal
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle offset of 1 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)31;

    seq_t a_nm2 = (seq_t)1;   /* a(n-2) for current iteration */
    seq_t a_nm1 = (seq_t)31;  /* a(n-1) for current iteration */
    seq_t a_n   = 0;          /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        /* a(n) = 62*a(n-1) - a(n-2) - 30   (all operations in seq_t) */
        a_n = (seq_t)62 * a_nm1 - a_nm2 - (seq_t)30;
        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
