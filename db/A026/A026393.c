/*
 * A026393  a(n) = T(n,n-2), where T is the array in A026386.
 * https://oeis.org/A026393
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^2 - 26*n + 24)/8 if n is even; a(n) = (9*n^2 - 20*n
 *            + 11)/8 if n is odd
 * Range:     u8 n <= 6, u16 n <= 231, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t n2 = nn * nn;
    seq_t top;
    if ((n & 1) == 0) {
        /* n even */
        top = (seq_t)9 * n2 - (seq_t)26 * nn + (seq_t)24;
    } else {
        /* n odd */
        top = (seq_t)9 * n2 - (seq_t)20 * nn + (seq_t)11;
    }
    return top / (seq_t)8;
}
