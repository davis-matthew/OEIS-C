/*
 * A092202  Expansion of (x - x^3) / (1 - x^5) in powers of x.
 * https://oeis.org/A092202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 1 if n % 5 == 1, -1 if n % 5 == 3, otherwise 0
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 5;
    if (r == 1)
        return (seq_t)1;
    if (r == 3)
        return (seq_t)-1;
    return (seq_t)0;
}
