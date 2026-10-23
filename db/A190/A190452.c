/*
 * A190452  E.g.f. exp(x+x^2/2+x^4/24).
 * https://oeis.org/A190452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=4 * a(n) = a(n-1) + (n-1)*a(n-2) +
 *            * ((n-3)*(n-2)*(n-1)/6)*a(n-4), n ≥ 4 * This recurrence follows
 *            directly from 6*a(n) = *
 *            6*a(n-1)+6*(n-1)*a(n-2)+(n-3)*(n-2)*(n-1)*a(n-4)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for NULL, size_t */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    /* Current values of a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a_nm4 = (seq_t)1;   /* a(0) */
    seq_t a_nm3 = (seq_t)1;   /* a(1) */
    seq_t a_nm2 = (seq_t)2;   /* a(2) */
    seq_t a_nm1 = (seq_t)4;   /* a(3) */
    seq_t an;                /* a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t term1 = a_nm1;                                   /* a(n-1) */
        seq_t term2 = (seq_t)(i - 1) * a_nm2;                  /* (n-1)*a(n-2) */
        seq_t num   = (seq_t)(i - 3) * (seq_t)(i - 2) * (seq_t)(i - 1);
        seq_t term3 = (num / 6) * a_nm4;                       /* ((n-3)*(n-2)*(n-1)/6)*a(n-4) */

        an = term1 + term2 + term3;

        /* Shift indices for next iteration */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }

    return an;
}
