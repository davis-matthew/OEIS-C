/*
 * A202865  Number of 3 X 3 0..n arrays with row and column sums one greater
 *          than the previous row and column.
 * https://oeis.org/A202865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n^5 + 15*n^4 + 15*n^3 - 5*n^2 - 8*n) / 10
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 71, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset=1, return 0 */

    seq_t nt = (seq_t)n;                   /* cast index to seq_t once */
    seq_t n2 = nt * nt;                    /* n^2 */
    seq_t n3 = n2 * nt;                    /* n^3 */
    seq_t n4 = n3 * nt;                    /* n^4 */
    seq_t n5 = n4 * nt;                    /* n^5 */

    seq_t term1 = (seq_t)3 * n5;           /* 3*n^5 */
    seq_t term2 = (seq_t)15 * n4;          /* 15*n^4 */
    seq_t term3 = (seq_t)15 * n3;          /* 15*n^3 */
    seq_t term4 = (seq_t)5 * n2;           /* 5*n^2 */
    seq_t term5 = (seq_t)8 * nt;           /* 8*n   */

    seq_t numerator = term1 + term2 + term3;
    numerator -= term4;
    numerator -= term5;

    return numerator / (seq_t)10;          /* exact division by 10 */
}
