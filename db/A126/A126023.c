/*
 * A126023  a(0)=0, a(1)=1; for n>1, a(n) = a(n-1)*(a(n-1)+a(n-2)).
 * https://oeis.org/A126023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1; for n>1, a(n) = a(n-1)*(a(n-1)+a(n-2))
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the two base cases directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* a(0) and a(1) are already known; iterate from 2 up to n */
    seq_t a_nm2 = (seq_t)0; /* a(n-2) */
    seq_t a_nm1 = (seq_t)1; /* a(n-1) */
    seq_t a_n   = (seq_t)0; /* a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(i) = a(i-1) * (a(i-1) + a(i-2)) */
        a_n = a_nm1 * (a_nm1 + a_nm2);

        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
