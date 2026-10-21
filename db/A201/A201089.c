/*
 * A201089  Number of -n..n arrays of 4 elements with first and second
 *          differences also in -n..n.
 * https://oeis.org/A201089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (101*n^4 + 202*n^3 + 190*n^2 + (92 or 86)*n + (24 or 21))
 *            / 24 with the linear and constant terms depending on parity: if
 *            n is even → 92*n + 24 if n is odd → 86*n + 21
 * Range:     u8 none, u16 n <= 4, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Convert to seq_t for arithmetic
    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;
    seq_t n3   = n2 * nn;
    seq_t n4   = n3 * nn;
    seq_t numerator;

    if ((n & 1) == 0) {            /* n even */
        numerator = 101ULL * n4
                   + 202ULL * n3
                   + 190ULL * n2
                   + 92ULL  * nn
                   + 24ULL;
    } else {                       /* n odd */
        numerator = 101ULL * n4
                   + 202ULL * n3
                   + 190ULL * n2
                   + 86ULL  * nn
                   + 21ULL;
    }

    return numerator / 24ULL;
}
