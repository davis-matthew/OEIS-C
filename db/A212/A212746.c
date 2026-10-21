/*
 * A212746  Number of (w,x,y,z) with all terms in {0,...,n} and at least one
 *          of them is the range of {w,x,y,z}.
 * https://oeis.org/A212746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For n even: a(n) = (15*n^3 + 3*n^2 + 12*n + 2) / 2 For n odd :
 *            a(n) = (15*n^3 + 15*n) / 2 (This is the closed form from the
 *            OEIS entry.)
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 730, u64 n <= 1000,
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
    if ((n & 1) == 0) {                /* n is even */
        seq_t t = (seq_t)15 * n3
                + (seq_t)3  * n2
                + (seq_t)12 * nn
                + (seq_t)2;
        return t / (seq_t)2;
    } else {                            /* n is odd */
        seq_t t = (seq_t)15 * n3
                + (seq_t)15 * nn;
        return t / (seq_t)2;
    }
}
