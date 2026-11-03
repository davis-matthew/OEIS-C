/*
 * A104859  Partial sums of A001764.
 * https://oeis.org/A104859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(3k,k)/(2k+1) This implementation
 *            uses the recurrence 2*(2*n^2+9*n+10)*a(n+2) -
 *            (31*n^2+99*n+80)*a(n+1) + 3*(9*n^2+27*n+20)*a(n) = 0 with
 *            a(0)=1, a(1)=2. The recurrence gives a(n) for any n ≥ 0 in O(n)
 *            time. No dynamic allocation is required, so the code works for
 *            all...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 48,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 178 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* Handle the first two terms directly. */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;

    /* a_nm2 = a_wide(k-2), a_nm1 = a_wide(k-1) */
    seq_t a_nm2 = (seq_t)1; /* a_wide(0) */
    seq_t a_nm1 = (seq_t)2; /* a_wide(1) */
    seq_t a_cur  = a_nm1;   /* a_wide(k) */

    /* Compute a_wide(k) for k = 2 .. n */
    for (idx_t k = (idx_t)1; k <= n - (idx_t)1; ++k) {
        /* recurrence with n = k-1 */
        seq_t N     = (seq_t)(k - (idx_t)1);
        seq_t N2    = N * N;                         /* n^2 */
        seq_t num1  = N2 * (seq_t)31
                    + N  * (seq_t)99
                    + (seq_t)80;                     /* (31*n^2+99*n+80) */
        seq_t num2  = N2 * (seq_t)9
                    + N  * (seq_t)27
                    + (seq_t)20;                     /* (9*n^2+27*n+20) */
        seq_t denom = (seq_t)2
                    * ((seq_t)2 * N2
                    + (seq_t)9 * N
                    + (seq_t)10);                    /* 2*(2*n^2+9*n+10) */
        seq_t numerator = num1 * a_nm1
                       - ((seq_t)3 * num2) * a_nm2;   /* (31...)*a_wide(n+1) - 3*(9...)*a_wide(n) */

        a_cur = numerator / denom;                   /* a_wide(k+1) */

        /* Shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }
    return a_cur;
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
