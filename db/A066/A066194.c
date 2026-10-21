/*
 * A066194  A permutation of the integers (a fractal sequence): a(n) =
 *          A006068(n-1) + 1.
 * https://oeis.org/A066194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = GrayToBinary(n-1)+1 * A006068 is the inverse of the
 *            binary reflected Gray code, * i.e. GrayToBinary(g) = the
 *            original integer whose Gray code is g. * Hence a(n) =
 *            GrayToBinary(n-1)+1 for n>=1. * This implementation works for
 *            all supported seq_t and idx_t types
 * Range:     u8 n <= 128, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For safety; the sequence is defined from n = 1 onward. */
    if (n == 0) return (seq_t)0;

    seq_t g = n - 1;      /* Gray code argument    */
    seq_t r = 0;          /* Result (binary integer) */

    for (seq_t t = g; t; t >>= 1) {
        r ^= t;
    }

    return r + (seq_t)1;
}
