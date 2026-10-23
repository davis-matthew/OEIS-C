/*
 * A049347  Period 3: repeat [1, -1, 0].
 * https://oeis.org/A049347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 1 if n mod 3 = 0, -1 if n mod 3 = 1, else 0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    if (r == 0)          return (seq_t)1;
    else if (r == 1)     return (seq_t)-1;
    else                 return (seq_t)0;
}
