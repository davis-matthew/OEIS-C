/*
 * A038376  a(n) = (n-3)*A006918(n-2)/2 for n >= 2, with a(0) = a(1) = 0.
 * https://oeis.org/A038376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - 3*n^3 - 4*n^2 + 12*n)/48 if n is even, a(n) = (n^4
 *            - 3*n^3 - n^2 + 3*n)/48 if n is odd. Offset: 0 (a(0) = 0)
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 352, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t n2 = (seq_t)n * (seq_t)n;
    seq_t n3 = n2 * (seq_t)n;
    seq_t n4 = n2 * n2;
    seq_t numerator;

    if (n & 1U) /* odd */
    {
        /* a(n) = (n^4 - 3*n^3 - n^2 + 3*n)/48 */
        numerator = n4 - (seq_t)3 * n3 - n2 + (seq_t)3 * (seq_t)n;
    }
    else /* even */
    {
        /* a(n) = (n^4 - 3*n^3 - 4*n^2 + 12*n)/48 */
        numerator = n4 - (seq_t)3 * n3 - (seq_t)4 * n2 + (seq_t)12 * (seq_t)n;
    }

    return numerator / (seq_t)48;
}
