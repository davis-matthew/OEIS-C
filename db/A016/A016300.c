/*
 * A016300  Inverse of 2291st cyclotomic polynomial.
 * https://oeis.org/A016300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is periodic with period 2291: a(n) = 1 for 0 <= (n mod
 *            2291) <= 28, 0 for 29 <= (n mod 2291) <= 78, -1 for 79 <= (n mod
 *            2291) <= 107, 0 otherwise
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const idx_t PERIOD = 2291;
    idx_t r = n % PERIOD;
    if (r <= 28) return 1;
    if (r <= 78) return 0;
    if (r <= 107) return -1;
    return 0;
}
