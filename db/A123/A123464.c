/*
 * A123464  Duplicate of A011782.
 * https://oeis.org/A123464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2^(n-1)
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 10, i64 n <= 10, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;       /* offset is 1, a(0) is undefined */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        res *= (seq_t)2;       /* multiply by 2 for each step */
    return res;
}
