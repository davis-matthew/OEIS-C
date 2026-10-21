/*
 * A081924  E.g.f.: exp(3*x)/(1-x)^2.
 * https://oeis.org/A081924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+4)*a(n-1) - 3*(n-1)*a(n-2) (sequence A081924)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0) = 1, a(1) = 5 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* Iterative computation of the recurrence */
    seq_t a_nm2 = (seq_t)1;  /* a(0) */
    seq_t a_nm1 = (seq_t)5;  /* a(1) */
    seq_t a_n   = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(i) = (i+4)*a(i-1) - 3*(i-1)*a(i-2) */
        a_n = (seq_t)(i + 4) * a_nm1 - (seq_t)3 * (seq_t)(i - 1) * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
