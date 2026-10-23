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
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;

    /* a_nm2 = a(k-2), a_nm1 = a(k-1) */
    seq_t a_nm2 = (seq_t)1; /* a(0) */
    seq_t a_nm1 = (seq_t)2; /* a(1) */
    seq_t a_cur  = a_nm1;   /* a(k) */

    /* Compute a(k) for k = 2 .. n */
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
                       - ((seq_t)3 * num2) * a_nm2;   /* (31...)*a(n+1) - 3*(9...)*a(n) */

        a_cur = numerator / denom;                   /* a(k+1) */

        /* Shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }
    return a_cur;
}
