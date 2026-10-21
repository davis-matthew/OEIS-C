/*
 * A099934  Antidiagonal sums of array A007754.
 * https://oeis.org/A099934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) - a(n-2) + 1 with initial values a(0)=1, a(1)=2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;

    seq_t a_nm2 = (seq_t)1;   /* a(0)          */
    seq_t a_nm1 = (seq_t)2;   /* a(1)          */

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        seq_t val = (seq_t)i * a_nm1;   /* n*a(n-1)      */
        val += (seq_t)1;                /* + 1            */
        val -= a_nm2;                   /* - a(n-2)       */
        a_nm2 = a_nm1;                  /* shift for next i */
        a_nm1 = val;
    }
    return a_nm1;
}
