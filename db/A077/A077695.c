/*
 * A077695  a(n) = triangular number whose index is n concatenated n times.
 * https://oeis.org/A077695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = T_{concatenate(n repeated n times)} where T_m = m*(m+1)/2
 *            is the m-th triangular number
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* number of decimal digits in n */
    idx_t tmp = n;
    unsigned int d = 0;
    do
    {
        d++;
        tmp /= 10;
    } while (tmp != 0);

    /* 10^d as seq_t */
    seq_t p = 1;
    for (unsigned int i = 0; i < d; ++i)
        p *= 10;

    /* concatenate n, n times */
    seq_t m = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        m = m * p + (seq_t)n;
    }

    /* triangular number */
    seq_t res = m * (m + 1) / 2;
    return res;
}
