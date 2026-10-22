/*
 * A003905  Degrees of irreducible representations of Janko group J2.
 * https://oeis.org/A003905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = degree of the n-th irreducible complex character of the
 *            Janko group J2, sorted in increasing order (finite sequence of
 *            21 terms, one per conjugacy class). The degrees are taken from
 *            the ATLAS character table of J2, which has no closed form. They
 *            are checked by the identity sum_{n=1..21} a(...
 * Range:     u8 n <= 18, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned short j2_degrees[21] = {
    1, 14, 14, 21, 21, 36, 63, 70, 70, 90, 126,
    160, 175, 189, 189, 224, 224, 225, 288, 300, 336
};

seq_t a(idx_t n)
{
    seq_t r = 0;
    if (n < 1 || n > 21)
        return r;
    r += (seq_t)j2_degrees[n - 1];
    return r;
}
