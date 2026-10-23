/*
 * A144635  a(n) = 5^n*Sum_{ k=0..n } binomial(2*k,k)/5^k.
 * https://oeis.org/A144635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): n*a(n) = (9*n-2)*a(n-1) - 10*(2*n-1)*a(n-2)
   with a(0) = 1, a(1) = 7 */

#include <stddef.h>

/* memoisation is avoided – the recurrence is computed directly
   in O(n) time, which is fast enough for n up to ~10000. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)7;

    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)7;   /* a(n-1) */
    seq_t a_n;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t nine_i = (seq_t)((long long)9 * i);          /* 9*i as seq_t */
        seq_t two_i_minus1 = (seq_t)((long long)2 * i - 1);
        seq_t term1 = (nine_i - 2) * a_nm1;                 /* (9*i-2)*a(i-1) */
        seq_t term2 = (seq_t)10 * two_i_minus1 * a_nm2;     /* 10*(2*i-1)*a(i-2) */
        a_n = (term1 - term2) / (seq_t)i;                   /* a(i) */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;   /* a(n) */
}
