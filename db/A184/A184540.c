/*
 * A184540  Number of (n+2) X 3 binary arrays with each 3 X 3 subblock having
 *          rows and columns in lexicographically nondecreasing order.
 * https://oeis.org/A184540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (84 + 149*n + 36*n^2 + n^3) / 6
 * Range:     u8 none, u16 n <= 30, u32 n <= 200, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to the sequence type for correct arithmetic */
    seq_t n_val = (seq_t)n;
    /* compute powers */
    seq_t n2 = n_val * n_val;      /* n^2 */
    seq_t n3 = n2 * n_val;         /* n^3 */
    /* accumulate the numerator */
    seq_t sum = (seq_t)84
              + (seq_t)149 * n_val
              + (seq_t)36 * n2
              + n3;
    /* divide by 6 */
    return sum / (seq_t)6;
}
