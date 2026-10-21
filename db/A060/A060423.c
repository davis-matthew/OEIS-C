/*
 * A060423  Number of obtuse triangles made from vertices of a regular n-gon.
 * https://oeis.org/A060423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = n*(n-1)*(n-3)/8 for n odd, * a(n) = n*(n-2)*(n-4)/8 for n
 *            even
 * Range:     u8 n <= 8, u16 n <= 42, u32 n <= 1878, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod;

    if (n & 1) /* odd */
        prod = nn * (nn - (seq_t)1) * (nn - (seq_t)3);
    else /* even */
        prod = nn * (nn - (seq_t)2) * (nn - (seq_t)4);

    return prod / (seq_t)8;
}
