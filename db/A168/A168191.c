/*
 * A168191  a(n) = n^3*(n^10 + 1)/2.
 * https://oeis.org/A168191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3*(n^10 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 31, u128 n <= 970,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to the sequence type */
    seq_t NN = (seq_t)n;

    /* powers of n */
    seq_t n2  = NN * NN;          /* n^2   */
    seq_t n3  = n2 * NN;          /* n^3   */
    seq_t n5  = n3 * n2;          /* n^5   */
    seq_t n10 = n5 * n5;          /* n^10  */

    /* split division by 2 to avoid intermediate overflow */
    seq_t half, res;
    if ((NN & 1) == 0) {          /* n is even – divide n^3 */
        half = n3 / 2;
        res  = half * (n10 + 1);
    } else {                     /* n is odd  – divide (n^10+1) */
        half = (n10 + 1) / 2;
        res  = n3 * half;
    }
    return res;
}
