/*
 * A014560  Inverse of 551st cyclotomic polynomial.
 * https://oeis.org/A014560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 551 < 19, * 0 if 19 ≤ n mod 551 < 29, * -1 if
 *            29 ≤ n mod 551 < 48, * 0 otherwise (period 551)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t period = 551;
    idx_t r = n % period;
    if (r < 19)          return (seq_t)1;
    else if (r < 29)     return (seq_t)0;
    else if (r < 48)     return (seq_t)-1;
    else                 return (seq_t)0;
}
