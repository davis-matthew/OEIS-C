/*
 * A014769  Squares of odd pentagonal numbers.
 * https://oeis.org/A014769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*n^4 + 60*n^3 + 37*n^2 + 10*n + 1 for n even; a(n) =
 *            36*n^4 - 12*n^3 + n^2 for n odd
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 104, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    if ((n & 1) == 0) {
        return 36 * n4 + 60 * n3 + 37 * n2 + 10 * nn + 1;
    } else {
        return 36 * n4 - 12 * n3 + n2;
    }
}
