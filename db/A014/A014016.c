/*
 * A014016  Expansion of inverse of 7th cyclotomic polynomial; period 7:
 *          repeat [1, -1, 0, 0, 0, 0, 0].
 * https://oeis.org/A014016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n % 7 == 0, -1 if n % 7 == 1, else 0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n % 7) {
        case 0:  return 1;
        case 1:  return -1;
        default: return 0;
    }
}
