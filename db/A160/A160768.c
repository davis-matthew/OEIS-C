/*
 * A160768  G.f.: (21+98*x+91*x^2+21*x^3+x^4)/(1-x)^5.
 * https://oeis.org/A160768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*n^4/3 + 233*n^3/6 + 214*n^2/3 + 373*n/6 + 21
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 144, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 nn = n;
    unsigned __int128 num = 58 * nn * nn * nn * nn
                           + 233 * nn * nn * nn
                           + 428 * nn * nn
                           + 373 * nn;
    num /= 6;
    num += 21;
    return (seq_t)num;
}
