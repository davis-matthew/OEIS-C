/*
 * A168503  Expansion of 1/(1-x/(1-2x/(1-3x/(1-x/(1-2x/(1-3x/(1-... (continued
 *          fraction).
 * https://oeis.org/A168503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=3, a(3)=15. For n ≥ 4: a(n) =
 *            ((13*n-5)*a(n-1) - 6*(8*n-13)*a(n-2) + 12*(5*n-13)*a(n-3) -
 *            12*(2*n-7)*a(n-4)) / (n+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)3;
    if (n == (idx_t)3) return (seq_t)15;

    /* Sliding window:  a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a_nm4 = (seq_t)1;  /* a(0) */
    seq_t a_nm3 = (seq_t)1;  /* a(1) */
    seq_t a_nm2 = (seq_t)3;  /* a(2) */
    seq_t a_nm1 = (seq_t)15; /* a(3) */

    seq_t a_n;                 /* a(n) to be computed */

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        /* Coefficient factors (all positive for i ≥ 4) */
        seq_t f1 = (seq_t)(13*i - 5);                 /* (13*i-5)   */
        seq_t f2 = (seq_t)(6 * (8*i - 13));           /* 6*(8*i-13) */
        seq_t f3 = (seq_t)(12 * (5*i - 13));          /* 12*(5*i-13)*/
        seq_t f4 = (seq_t)(12 * (2*i - 7));           /* 12*(2*i-7) */

        /* Compute the terms of the numerator using only unsigned arithmetic. */
        seq_t term1 = f1 * a_nm1;  /* (13*i-5)*a(n-1) */
        seq_t term2 = f2 * a_nm2;  /* 6*(8*i-13)*a(n-2) */
        seq_t term3 = f3 * a_nm3;  /* 12*(5*i-13)*a(n-3) */
        seq_t term4 = f4 * a_nm4;  /* 12*(2*i-7)*a(n-4)  */

        /* Separate positive and negative contributions to avoid temporary negatives. */
        seq_t pos = term1 + term3; /* positive part */
        seq_t neg = term2 + term4; /* negative part */

        /* Numerator is difference of the two parts. */
        seq_t numer = pos - neg;

        /* Exact division: (n+1) divides the numerator by construction. */
        a_n = numer / (seq_t)(i + 1);

        /* Shift the sliding window for next iteration. */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
