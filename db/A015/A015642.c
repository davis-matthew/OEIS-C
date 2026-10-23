/*
 * A015642  Inverse of 1633rd cyclotomic polynomial.
 * https://oeis.org/A015642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = 1 if r∈[0,22]; 0 if r∈[23,70] or [94,1632]; -1 if
 *            r∈[71,93]; with r = n mod 1633
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 1633ULL;
    if (r < 23ULL)   return (seq_t)1;
    if (r < 71ULL)   return (seq_t)0;
    if (r < 94ULL)   return (seq_t)-1;
    return (seq_t)0;
}
