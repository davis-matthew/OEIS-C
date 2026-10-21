/*
 * A081587  Third row of Pascal-(1,4,1) array A081579.
 * https://oeis.org/A081587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2 - 5*n + 25*n^2)/2
 * Range:     u8 n <= 3, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* Compute the numerator 25*n^2 - 5*n + 2; all operations are on seq_t to avoid overflow */
    seq_t num = (seq_t)25 * nn * nn - (seq_t)5 * nn + (seq_t)2;

    /* Divide by 2, the result is always even so integer division is exact */
    return num / 2;
}
