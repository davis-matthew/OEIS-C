/*
 * A084177  Binomial transform of Jacobsthal oblongs.
 * https://oeis.org/A084177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*5^n - (-1)^n - 2^n)/9 Recurrence: a(0)=0, a(1)=1,
 *            a(2)=5, a(n)=6*a(n-1) - 3*a(n-2) - 10*a(n-3) (n≥3)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for idx_t, seq_t supplied by the build */

seq_t a(idx_t n)
{
    /* Base cases already known */
    if (n == 0U) return (seq_t)0;
    if (n == 1U) return (seq_t)1;
    if (n == 2U) return (seq_t)5;

    /* Use three previous terms for the recurrence */
    seq_t a_nm3 = (seq_t)0;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)5;   /* a(n-1) */
    seq_t a_n   = (seq_t)0;   /* a(n)   */

    for (idx_t i = 3U; i <= n; ++i) {
        a_n = (seq_t)(6 * a_nm1) - (seq_t)(3 * a_nm2) - (seq_t)(10 * a_nm3);

        /* Shift window for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
