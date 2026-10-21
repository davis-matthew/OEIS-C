/*
 * A215919  a(n) = -3*a(n-1) + a(n-3), with a(0)=0, a(1)=-3, a(2)=12.
 * https://oeis.org/A215919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -3*a(n-1) + a(n-3) with a(0) = 0, a(1) = -3, a(2) = 12
 *            This implementation computes any term on demand using an O(n)
 *            recurrence without storing a full cache, so it works for
 *            arbitrary-precision, 64‑bit, or 128‑bit seq_t types
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 40, i128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return -(seq_t)3;
    if (n == 2) return (seq_t)12;

    seq_t a_nm3 = (seq_t)0;           /* a(n-3)   */
    seq_t a_nm2 = -(seq_t)3;          /* a(n-2)   */
    seq_t a_nm1 = (seq_t)12;          /* a(n-1)   */
    seq_t an    = 0;

    for (idx_t i = 3; i <= n; ++i) {
        an = -(seq_t)3 * a_nm1 + a_nm3;   /* a(n) = -3*a(n-1)+a(n-3) */
        a_nm3 = a_nm2;                    /* shift window forward   */
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return an;
}
