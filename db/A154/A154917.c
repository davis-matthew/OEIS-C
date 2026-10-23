/*
 * A154917  Duplicate of A001045.
 * https://oeis.org/A154917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (2^n - (-1)^n)/3
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        v = v * 2 + ((i & 1) ? (seq_t)1 : (seq_t)-1);
    }
    return v;
}
