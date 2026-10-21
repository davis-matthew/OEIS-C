/*
 * A224999  Duplicate of A001651.
 * https://oeis.org/A224999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n + floor((n-1)/2)
 * Range:     i8 n <= 66, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This yields the sequence of positive integers that are not divisible by 3 (duplicate of A001651). */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn + ( (nn - 1) / 2 );
    return res;
}
