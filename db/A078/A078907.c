/*
 * A078907  Expansion of modular function j/256 in powers of m=k^2=lambda(t).
 * https://oeis.org/A078907
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n for n > 2, with special values a(-2) = 1, a(-1) = -1,
 *            a(0) = 3, a(1) = 0, a(2) = 3
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
    case -2: return (seq_t)1;
    case -1: return (seq_t)-1;
    case 0:  return (seq_t)3;
    case 1:  return (seq_t)0;
    case 2:  return (seq_t)3;
    default: return (seq_t)n;
    }
}
