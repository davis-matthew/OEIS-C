/*
 * A099504  Expansion of 1/(1-5*x+x^3).
 * https://oeis.org/A099504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - a(n-3) for n >= 3, with initial values a(0)=1,
 *            a(1)=5, a(2)=25
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)25;

    /* Keep the last three computed values */
    seq_t a_nm3 = (seq_t)1;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)5;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)25;  /* a(n-1) */
    seq_t cur;

    /* Iterate from 3 up to n */
    for (idx_t i = 3; i <= n; ++i)
    {
        cur = (seq_t)5 * a_nm1 - a_nm3;
        /* Shift the window of last three values */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return a_nm1;
}
