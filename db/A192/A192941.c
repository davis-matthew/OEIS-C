/*
 * A192941  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x)=(2x+1)(2x+2)...(2x+n).
 * https://oeis.org/A192941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, for n>=2 * a(n) = (2n+1) * a(n-1) - (n^2-5) *
 *            a(n-2) * This recurrence follows from the conjecture *
 *            a(n)+(-2*n-1)*a(n-1)+(n^2-5)*a(n-2)=0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)1;   /* a(1) */
    seq_t a_curr = a_nm1;     /* temporary holder */

    for (idx_t k = 2; k <= n; ++k)
    {
        /* Difference n^2 - 5, kept as a signed 64-bit value to avoid
         * unsigned underflow when n < 3. */
        int64_t diff = (int64_t)k * (int64_t)k - 5;

        /* term = (2k+1) * a_nm1 */
        a_curr = (seq_t)((idx_t)2 * k + (idx_t)1) * a_nm1;

        if (diff >= 0)
        {
            /* Subtract diff * a_nm2 when diff is non‑negative */
            a_curr = a_curr - (seq_t)(diff) * a_nm2;
        }
        else
        {
            /* Add (-diff) * a_nm2 when diff is negative */
            a_curr = a_curr + (seq_t)(-diff) * a_nm2;
        }

        /* Shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_curr;
    }

    return a_curr;
}
