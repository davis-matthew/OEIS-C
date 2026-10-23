/*
 * A198636  One half of total number of round trips, each of length 2n, on the
 *          graph P_6 (o-o-o-o-o-o).
 * https://oeis.org/A198636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + a(n-3), with a(0)=3, a(1)=5,
 *            a(2)=13:
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* one half of the total number of round trips, each of length 2n, on the graph P6. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)13;

    seq_t a_nm3 = (seq_t)3;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)5;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)13;  /* a(n-1) */
    seq_t a_n = 0;

    for (idx_t i = 3; i <= n; ++i) {
        a_n = (seq_t)5 * a_nm1 - (seq_t)6 * a_nm2 + a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
