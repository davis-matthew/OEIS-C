/*
 * A135405  Sequence where the sum of each pair of consecutive elements is a
 *          square.
 * https://oeis.org/A135405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*(n+1)/2 + 2*(-1)^n for n>0, a(0)=0
 * Range:     u8 n <= 21, u16 n <= 360, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    seq_t nseq = (seq_t)n;
    seq_t tri = ((nseq + 2) * (nseq + 1)) / 2;
    /* tri is (n+2)*(n+1)/2.  Add 2 if n is even, subtract 2 if n is odd. */
    if (n & 1U) { /* odd n: (-1)^n = -1, so add 2*(-1) = -2 */
        return tri - (seq_t)2;
    } else {      /* even n: (-1)^n = 1, so add 2*1 = 2 */
        return tri + (seq_t)2;
    }
}
