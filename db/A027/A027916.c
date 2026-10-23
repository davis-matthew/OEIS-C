/*
 * A027916  Least k such that 1+2+...+k >= E{1,2,...,n}, where E = 2nd
 *          elementary symmetric function.
 * https://oeis.org/A027916
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 + n - 2) / 2 - floor((n-1) / 3) , n ≥ 2
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t num   = n_seq * n_seq + n_seq - (seq_t)2;   /* n^2 + n - 2  */
    seq_t half  = num / (seq_t)2;                     /* integer division truncates toward 0, works here */
    seq_t sub   = (n - (idx_t)1) / (idx_t)3;          /* floor((n-1)/3)  */
    return half - sub;
}
