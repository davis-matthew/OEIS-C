/*
 * A070300  Minimal number of 0's in a 2n X 2n (0,1) matrix that contains no n
 *          X n submatrix of 1's.
 * https://oeis.org/A070300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n + 1
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the result type to keep all operations in the
       precision of seq_t, as required. */
    seq_t nn = (seq_t)n;
    return 3 * nn + 1;
}
