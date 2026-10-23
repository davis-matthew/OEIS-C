/*
 * A173714  a(n) = floor(Lucas(n+1)/2), Lucas(n) = A000032(n).
 * https://oeis.org/A173714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2) + (n mod 3 == 2 ? 1 : 0) with initial
 *            values a(0)=0, a(1)=1 (equivalently a(n) = floor(Lucas(n+1)/2))
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial small cases directly */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Two successive values enough to iterate the recurrence */
    seq_t a_nm2 = (seq_t)0;   /* a(0) */
    seq_t a_nm1 = (seq_t)1;   /* a(1) */
    seq_t a_n    = (seq_t)0;  /* placeholder for current term */

    for (idx_t i = 2; i <= n; ++i) {
        a_n = a_nm2 + a_nm1;                    /* a(n-1)+a(n-2)  */
        if ((i % 3) == 2)                       /* extra +1 when needed */
            a_n += (seq_t)1;
        a_nm2 = a_nm1;                          /* shift window */
        a_nm1 = a_n;
    }
    return a_n;
}
