/*
 * A162942  a(n) = binomial(n+1,2)*7^2.
 * https://oeis.org/A162942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 49 * n * (n+1) / 2
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t tri;
    if ((nn & 1) == 0)          /* n is even */
        tri = (nn / 2) * (nn + 1);
    else                         /* n is odd */
        tri = nn * ((nn + 1) / 2);
    return tri * 49;
}
