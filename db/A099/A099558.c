/*
 * A099558  Antidiagonal sums of the triangle A099557.
 * https://oeis.org/A099558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) for n ≥ 5; a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=4
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 52, u64 n <= 105,
 *            u128 n <= 209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases   */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)4;

    /* iteratively compute using the recurrence:
       a_i = 2*a_{i-1} - a_{i-2} + a_{i-5}   */
    seq_t a_nm5 = (seq_t)1; /* a(i-5)   */
    seq_t a_nm4 = (seq_t)1; /* a(i-4)   */
    seq_t a_nm3 = (seq_t)2; /* a(i-3)   */
    seq_t a_nm2 = (seq_t)3; /* a(i-2)   */
    seq_t a_nm1 = (seq_t)4; /* a(i-1)   */
    seq_t a_n;

    for (idx_t i = 5; i <= n; ++i)
    {
        a_n =  (seq_t)2 * a_nm1 - a_nm2 + a_nm5;

        /* shift window forward */
        a_nm5 = a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
