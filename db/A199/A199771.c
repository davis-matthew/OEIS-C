/*
 * A199771  Row sums of the triangle in A199332.
 * https://oeis.org/A199771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3+2*n^2+4*n+(-1)^n)/8
 * Range:     u8 n <= 9, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    if (n & 1) {                  /* n is odd */
        seq_t t = nseq + 1;       /* t = n + 1, even */
        return nseq * t * t / (seq_t)4;
    } else {                      /* n is even */
        seq_t t = nseq * nseq + (seq_t)2 * nseq + 2;
        return nseq * t / (seq_t)4;
    }
}
