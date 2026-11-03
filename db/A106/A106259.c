/*
 * A106259  Expansion of 1/sqrt(1-12x-12x^2).
 * https://oeis.org/A106259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   n*a(n) = 6*(2*n-1)*a(n-1) + 12*(n-1)*a(n-2); with a(0)=1, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 200 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

seq_t a_wide(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;

    seq_t a_nm2 = (seq_t)1;   /* a_wide(0) */
    seq_t a_nm1 = (seq_t)6;   /* a_wide(1) */
    seq_t a_n   = a_nm1;      /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute a_wide(i) using the recurrence */
        seq_t term1 = (seq_t)(12 * i - 6) * a_nm1;
        seq_t term2 = (seq_t)12 * (i - 1) * a_nm2;
        a_n = (term1 + term2) / i;
        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
