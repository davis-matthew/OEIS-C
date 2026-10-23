/*
 * A015259  Gaussian binomial coefficient [ n,2 ] for q = -8.
 * https://oeis.org/A015259
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 57*a(n-1) + 456*a(n-2) - 512*a(n-3); a(2)=1, a(3)=57,
 *            a(4)=3705
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 2; values for n >= 2 are computed by the recurrence. */
    if (n < 2) return 0;

    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)57;
    if (n == 4) return (seq_t)3705;

    seq_t a_nm3 = (seq_t)1;        /* a(2)   */
    seq_t a_nm2 = (seq_t)57;       /* a(3)   */
    seq_t a_nm1 = (seq_t)3705;     /* a(4)   */
    seq_t curr;

    for (idx_t i = 5; i <= n; ++i) {
        curr = (seq_t)57 * a_nm1 + (seq_t)456 * a_nm2;
        curr -= (seq_t)512 * a_nm3;   /* subtraction is safe: result is positive */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = curr;
    }
    return a_nm1;
}
