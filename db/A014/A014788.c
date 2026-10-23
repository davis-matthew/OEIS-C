/*
 * A014788  Inverse of 779th cyclotomic polynomial.
 * https://oeis.org/A014788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 779 ∈ [0,18]; a(n) = -1 if n mod 779 ∈
 *            [41,59]; a(n) = 0 otherwise
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 779;
    if (r <= 18)
        return (seq_t)1;
    if (r >= 41 && r <= 59)
        return (seq_t)-1;
    return (seq_t)0;
}
