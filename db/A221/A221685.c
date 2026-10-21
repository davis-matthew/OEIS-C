/*
 * A221685  Number of 0..n arrays of length 6 with each element differing from
 *          at least one neighbor by 1 or less, starting with 0.
 * https://oeis.org/A221685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*n^2 + 57*n - 99 for n>4 a(1)=32, a(2)=113, a(3)=241,
 *            a(4)=418
 * Range:     u8 n <= 3, u16 n <= 58, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small n explicitly */
    if (n <= 4) {
        static const seq_t start[5] = { 0, 32, 113, 241, 418 };
        return start[n];
    }

    /* Compute the quadratic formula in seq_t */
    seq_t nn = (seq_t)n;       /* cast index to seq_t */
    seq_t val = nn * nn;       /* n^2     */
    val *= 18;                 /* 18*n^2  */
    val += nn * 57;            /* +57*n   */
    val -= 99;                 /* -99     */
    return val;
}
