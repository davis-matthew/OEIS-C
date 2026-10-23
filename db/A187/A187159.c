/*
 * A187159  Number of 6-step one space at a time bishop's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = { 0 for n=1..3, * 56 for n=4, * 284*n^2 - 1992*n + 3316
 *            for n>=5 }
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) {
        if (n <= 3) return (seq_t)0;
        return (seq_t)56;
    }
    seq_t nn = (seq_t)n;
    return (seq_t)284 * nn * nn - (seq_t)1992 * nn + (seq_t)3316;
}
