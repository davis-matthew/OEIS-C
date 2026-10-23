/*
 * A101100  The first summation of row 5 of Euler's triangle - a row that will
 *          recursively accumulate to the power of 5.
 * https://oeis.org/A101100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{j=1..min(5,n)} Eulerian(5, j-1) Eulerian numbers for
 *            m = 5 are [1, 26, 66, 26, 1]
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t euler[5] = { (seq_t)1, (seq_t)26, (seq_t)66, (seq_t)26, (seq_t)1 };

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n < 1 (unused) returns 0. */
    if (n == 0) return (seq_t)0;

    idx_t limit = (n < 5 ? n : 5);   /* j runs from 1 to min(5,n) */
    seq_t sum = 0;
    for (idx_t i = 0; i < limit; ++i)
        sum += euler[i];
    return sum;
}
