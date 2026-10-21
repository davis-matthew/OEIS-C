/*
 * A214994  Power ceiling sequence of (golden ratio)^5.
 * https://oeis.org/A214994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 10*a(n-2) - a(n-3), a(0) = 12, a(1) = 134,
 *            a(2) = 1487
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the initial three terms directly. */
    if (n == 0) return (seq_t)12;
    if (n == 1) return (seq_t)134;
    if (n == 2) return (seq_t)1487;

    /* Use a sliding window of the last three values. */
    seq_t a_nm3 = (seq_t)12;          /* a(n-3) */
    seq_t a_nm2 = (seq_t)134;         /* a(n-2) */
    seq_t a_nm1 = (seq_t)1487;        /* a(n-1) */
    seq_t a_n = a_nm1;                /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        a_n = (seq_t)(12) * a_nm1 - (seq_t)(10) * a_nm2 - a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
