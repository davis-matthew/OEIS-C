/*
 * A201695  Number of n X 3 0..2 arrays with rows and columns
 *          lexicographically nondecreasing read forwards and nonincreasing
 *          read backwards.
 * https://oeis.org/A201695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (9*n^4 + 8*n^3 - 24*n^2 - 29*n + 54) / 6
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 166, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the value type for arithmetic */
    seq_t nn   = (seq_t)n;
    seq_t nn2  = nn * nn;
    seq_t nn3  = nn2 * nn;
    seq_t nn4  = nn3 * nn;

    seq_t num  = 9 * nn4 + 8 * nn3 - 24 * nn2 - 29 * nn + 54;
    return num / 6;
}
