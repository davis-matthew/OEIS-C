/*
 * A167576  The first column of the ED3 array A167572.
 * https://oeis.org/A167576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=5; for n>=3 * a(n) = 4*a(n-1) + (4*n^2 - 16*n +
 *            15)*a(n-2) (recurrence)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)       /* undefined for n<1, return 0 as safeguard */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)5;

    seq_t a_prev2 = (seq_t)1;   /* a(1) */
    seq_t a_prev1 = (seq_t)5;   /* a(2) */
    seq_t a_curr  = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* compute coefficient as signed to avoid unsigned underflow */
        long long coeff_ll = 4LL * (long long)i * (long long)i
                            - 16LL * (long long)i + 15LL;
        seq_t coeff = (seq_t)coeff_ll;

        a_curr = 4 * a_prev1 + coeff * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
