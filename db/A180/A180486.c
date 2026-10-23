/*
 * A180486  Numbers of the form ceiling(A179896(j)/A018252(j)) where
 *          A179896(j) mod A018252(j) <> 0.
 * https://oeis.org/A180486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 + 3*n
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast each operand to seq_t to avoid intermediate integer promotion issues */
    return (seq_t)2 + (seq_t)3 * (seq_t)n;
}
