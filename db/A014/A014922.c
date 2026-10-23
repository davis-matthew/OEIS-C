/*
 * A014922  Inverse of 913th cyclotomic polynomial.
 * https://oeis.org/A014922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Inverse of 913th cyclotomic polynomial
   a(0) = 1
   a(1) = 1
   ...
   a(10) = 1
   a(11) = 0
   ...
   a(82) = 0
   a(83) = -1
   ...
   a(93) = -1
   a(94) = 0
   ...
   Period length = 913

   Formula: a(n) = 1   if (n mod 913) <= 10,
                -1  if 83 <= (n mod 913) <= 93,
                0   otherwise.
   (This matches the recurrence coming from 1/Φ_913(x)
    expanded as a formal power series.) */

seq_t a(idx_t n)
{
    const idx_t period = 913;
    idx_t r = n % period;

    if (r <= 10)
        return (seq_t)1;
    if (r <= 82)
        return (seq_t)0;
    if (r <= 93)
        return (seq_t)-1;
    return (seq_t)0;
}
