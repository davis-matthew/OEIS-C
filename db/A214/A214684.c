/*
 * A214684  a(1)=1, a(2)=1, and, for n>2, a(n)=(a(n-1)+a(n-2))/5^k, where 5^k
 *          is the highest power of 5 dividing a(n-1)+a(n-2).
 * https://oeis.org/A214684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n-1)+a(n-2))/5^k, a(1) = 1, a(2) = 1, where 5^k is the
 *            largest power of 5 dividing a(n-1)+a(n-2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1 || n == 2)
        return (seq_t)1;

    seq_t a_prev2 = (seq_t)1;   /* a(n-2) */
    seq_t a_prev1 = (seq_t)1;   /* a(n-1) */
    seq_t a_curr   = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t sum = a_prev1 + a_prev2;

        /* Remove the highest power of 5 dividing sum. */
        while (sum % (seq_t)5 == (seq_t)0)
            sum /= (seq_t)5;

        a_curr = sum;

        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
