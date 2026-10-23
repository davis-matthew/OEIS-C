/*
 * A139597  A139278(n) followed by A139274(n+1).
 * https://oeis.org/A139597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3-3*(-1)^n + 14*n + 8*n^2) / 4
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A139597 (offset 0) */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* 3-3*(-1)^n is 0 for even n, 6 for odd n */
    seq_t term = (nn & (seq_t)1) ? (seq_t)6 : (seq_t)0;
    term += (seq_t)14 * nn;
    term += (seq_t)8 * nn * nn;
    return term / (seq_t)4;
}
