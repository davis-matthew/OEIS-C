/*
 * A121351  a(n) = (3*n+1)*a(n-1) - a(n-2), starting a(0)=0, a(1)=1.
 * https://oeis.org/A121351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)*a(n-1) - a(n-2), with a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
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

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t an = a1;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t coef = (seq_t)3 * i + 1;   /* (3*i)+1 as seq_t */
        an = coef * a1 - a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
