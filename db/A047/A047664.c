/*
 * A047664  Row 7 of square array defined in A047662.
 * https://oeis.org/A047664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n/315) * (4*n^6 + 14*n^5 + 112*n^4 + 245*n^3 + 616*n^2 +
 *            686*n + 528)
 * Range:     u8 none, u16 n <= 2, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
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
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;
    seq_t poly = 4 * n6 + 14 * n5 + 112 * n4 + 245 * n3 + 616 * n2 + 686 * nn + 528;
    seq_t numer = nn * poly;
    return numer / 315;
}
