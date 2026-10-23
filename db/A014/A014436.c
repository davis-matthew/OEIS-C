/*
 * A014436  Inverse of 427th cyclotomic polynomial.
 * https://oeis.org/A014436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 427 in [0,6], -1 if n mod 427 in [61,67], 0
 *            otherwise
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 427ULL;
    if (r < 7ULL)
        return 1;
    else if (r < 61ULL)
        return 0;
    else if (r < 68ULL)
        return (seq_t)(-1);
    else
        return 0;
}
