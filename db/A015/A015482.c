/*
 * A015482  q-Fibonacci numbers for q=10, scaling a(n-1).
 * https://oeis.org/A015482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(n-1)*a(n-1) + a(n-2) Initial values: a(0)=0, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t a_nm2 = (seq_t)0;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)1;   /* a(n-1) */
    seq_t pow10  = (seq_t)1;  /* will hold 10^(i-1) */

    for (idx_t i = 2; i <= n; ++i) {
        pow10 *= (seq_t)10;          /* 10^(i-1) */
        seq_t a_n = pow10 * a_nm1 + a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
