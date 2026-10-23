/*
 * A131756  Period 3: repeat [2, -1, 3].
 * https://oeis.org/A131756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-3) for n>2; a(0)=2, a(1)=-1, a(2)=3 (period 3)
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[3] = { (seq_t)2, (seq_t)-1, (seq_t)3 };
    return tbl[n % 3];
}
