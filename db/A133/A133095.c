/*
 * A133095  Row sums of triangle A133094.
 * https://oeis.org/A133095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*2^(n-1) - n, for n ≥ 2; a(1) = 1. This is equivalent to
 *            the recurrence a(n) = 4*a(n-1) - 5*a(n-2) + 2*a(n-3) for n ≥ 5
 *            with initial terms a(1)=1, a(2)=4, a(3)=9, a(4)=20
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n < 1, return 0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)9;
    if (n == 4) return (seq_t)20;

    /* use the recurrence to compute from n = 5 upwards */
    seq_t a_nm3 = (seq_t)4;   /* a(2) */
    seq_t a_nm2 = (seq_t)9;   /* a(3) */
    seq_t a_nm1 = (seq_t)20;  /* a(4) */
    seq_t a_val = a_nm1;      /* current term */

    for (idx_t k = 5; k <= n; ++k) {
        a_val = (seq_t)4 * a_nm1 - (seq_t)5 * a_nm2 + (seq_t)2 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_val;
    }
    return a_val;
}
